#ifndef __ROUTER_H_
#define __ROUTER_H_

typedef struct route_info {
    int paths = 0;
    String http_method;
    String* components;
} route_info;

class Router {
    private:
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
        route_info parse(String route) {
            route_info result;
            
            const char* route_str = route.c_str();
            int index = 0, component_index = 0, spaces = 0;
            char tmp = *(route_str + index++);
            String buf = "";

            // count paths
            result.paths = this->count_paths(route_str);
            
            // parse paths
            result.components = new String[result.paths];
            while (tmp != '\0') {
                if (tmp == ' ') {
                  // If it's the first space, then we know it is the http_method
                  if (spaces++ == 0) {
                    result.http_method = buf;
                    buf = "";
                  }
                }
                else if (spaces > 0 && tmp == '/') {
                    if (buf.length() > 0 && component_index < result.paths) {
                        *(result.components + component_index++) = buf;
                    }
                    buf = "";
                } else {
                    buf += tmp;
                }

                if (spaces == 2) break;
                tmp = *(route_str + index++);
            }
            
            if (buf.length() > 0 && component_index < result.paths) {
                *(result.components + component_index++) = buf;
            }

            return result;
        }

        void destroy(route_info info) {
            free(info.components);
        }
};

#endif
