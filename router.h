#ifndef __ROUTER_H_
#define __ROUTER_H_

#define MAXIMUM_COMPONENTS 10
#define MAXIMUM_CHARACTERS 20

// These are some globals used to help reduce memory requirements.
// by sharing this buffer, I was able to reduce memory footprint by
// a crazy amount.
int path_index = 0;
int component_buffer_index = 0;
char component_buffer[MAXIMUM_COMPONENTS * MAXIMUM_CHARACTERS];


// route_info
// This is a struct which we ideally will only ever instantiate once.
// It has two properties
//  - paths         qty of components in a given route which we are parsing
//  - components    actual route components.
//
// There is an important thing to note. Arbitrarily, this is only designed to
// support, at most, 10 distinct routes within a given path. Each component
// has a 20 character maximum.  An example of something which is NOT support
//
// /unfortunate-long-name/visibility/1 - first component exceeds 20 character limit!
// 
// So... just keep that in mind.
typedef struct route_info {
    int paths = 0;
    char* components[MAXIMUM_COMPONENTS];
} route_info;

// This method will clear the component buffer.
void clr_c_buf() {
  path_index = 0;
  component_buffer_index = 0;
  for (int i = 0; i < MAXIMUM_COMPONENTS * MAXIMUM_CHARACTERS; i++) {
    *(component_buffer + i) = '\0';
  }
}

class Router {
    private:
        // only have 1 instance ever, for memory reasons.
        route_info current_route;
        
        // given a c-style string, determine the amount of paths
        // (forward-slashes) which exist.
        int count_paths(const char* route_str) {
            int index = 0, paths = 0, spaces = 0;
            char tmp = *(route_str + index++);
            // First we need to count how many slashes there are
            while (tmp != '\0') {
                if (tmp == ' ') spaces++;
                else if (spaces > 1) break;
                else if (spaces > 0 && tmp == '/') paths++;
                tmp = *(route_str + index++);
            }
            return paths;
        }

    public:
      Router() {
        this->current_route.paths = 0;
        for (int i = 0; i < MAXIMUM_COMPONENTS; i++) {
          this->current_route.components[i] = (component_buffer + (MAXIMUM_CHARACTERS * i));
        }
      }
      
      route_info parse(char* route_str) {
          clr_c_buf();
          
          int spaces = 0, index = 0;
          char tmp = *(route_str + index++);

          // count paths
          this->current_route.paths = this->count_paths(route_str);          
          while (tmp != '\0' && path_index < MAXIMUM_COMPONENTS) {
            // Eat everything until the first space
            if (spaces == 0 && tmp != ' ') {
              // skip
            } else if (tmp == ' ') {
              // We have encountered a space.
              if (spaces++ != 0) break;
            } else if (tmp == '/') {
              // We have reached the end of a position.
              if (MAXIMUM_CHARACTERS * path_index == component_buffer_index) {
                // We need to skip this because the buffer is effectively empty.
                // This would imply the slash we encountered is either at the beginning
                // or is doubled up and, therfore, meaningless.
              } else {
                // We have encountered the end of a component and all is good.
                // Advance the array pointer such that it now starts at
                // the beginning of our next buffer block, per the index of
                // our path.
                component_buffer_index = MAXIMUM_CHARACTERS * ++path_index;
              }
            } else {
              // We are on a regular old character.
              component_buffer[component_buffer_index++] = tmp;
            }
            tmp = *(route_str + index++);
          }

          return this->current_route;
      }
};

#endif
