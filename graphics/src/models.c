#include "models.h"

#include <log.h>
#include <tinyobj_loader_c.h>
#include <string.h>


//int load_model(const char* filename) {
//    tinyobj_attrib_t attrib;
//    tinyobj_shape_t* shapes = NULL;
//    size_t num_shapes;
//    tinyobj_material_t* materials = NULL;
//    size_t num_materials;
//
//    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
//    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
//                                &num_materials, filename, get_file_data, flags);
//    if (ret != TINYOBJ_SUCCESS) {
//        log_error("Error: %d", ret);
//        return 1;
//    }
//
//    log_info("# of shapes    = %d\n", (int)num_shapes);
//    log_info("# of materials = %d\n", (int)num_materials);
//
//}