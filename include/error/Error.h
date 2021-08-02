/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef CUPCFD_ERROR_INCLUDE_H
#define CUPCFD_ERROR_INCLUDE_H

#include <iostream>
#include <string>
#include <unordered_map>

namespace cupcfd::error {
// ToDo: Would like to revisit this to enhance error handling.
// Ideally some form of class or switch to exceptions, but would like to
// avoid overhead of copying object files around

// For now, we will just rely on a large collection of error codes and
// accompanying static messages
// ToDo: May be nice at some point to encapsulate errors alongside the
// 'packages' they are associated with.

enum eCodes {
  E_ADJACENCY_LIST_SUCCESS,
  E_ADJACENCY_LIST_INVALID_INDEX,
  E_ADJACENCY_LIST_NODE_EXISTS,
  E_ADJACENCY_LIST_NODE_MISSING,
  E_ADJACENCY_LIST_DISTRIBUTED_FINALIZED,
  E_ADJACENCY_LIST_DISTRIBUTED_NOTFINALIZED,
  E_ADJACENCY_LIST_EDGE_EXISTS,
  E_ADJACENCY_LIST_NODE_CLAIM_MISMATCH,
  E_SOURCE_MISSING,
  E_BUILDER_BASE_CLASS_ERR,
  E_FILE_MISSING,
  E_GEOMETRY_NO_INTERSECT,
  E_GEOMETRY_POLYHEDRON_MISMATCH,
  E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION,
  E_GEOMETRY_NVERT_MISMATCH,
  E_GEOMETRY_NEDGE_MISMATCH,
  E_GEOMETRY_NFACE_MISMATCH,
  E_GEOMETRY_NO_VALID_CELL,
  E_GEOMETRY_ON_EDGE,
  E_GEOMETRY_ON_VERTEX,
  E_GEOMETRY_LOGIC_ERROR,
  E_GEOMETRY_ZERO_AREA,
  E_GEOMETRY_NAN_AREA,
  E_ARRAY_SUCCESS,
  E_ARRAY_SIZE_UNDERSIZED,
  E_ARRAY_SIZE_MISMATCH,
  E_ARRAY_UNKNOWN,
  E_SEARCH_SUCCESS,
  E_SEARCH_NOT_FOUND,
  E_SEARCH_UNSORTED,
  E_SEARCH_SORT_CHECK_FAILURE,
  E_SEARCH_UNKNOWN,
  E_SORT_ERROR,
  E_PARTICLE_CELL_NOT_FOUND,
  E_PARTITIONER_SUCCESS,
  E_PARTITIONER_UNDERSIZED_ARRAY,
  E_PARTITIONER_MISSING_NODE_DATA,
  E_PARTITIONER_NO_RESULTS,
  E_PARTITIONER_NPARTS_UNSET,
  E_PARMETIS_NCON_UNSET,
  E_PARMETIS_NPARTS_UNSET,
  E_PARMETIS_ERROR,
  E_PARMETIS_SUCCESS,
  E_METIS_SUCCESS,
  E_METIS_ERROR,
  E_MATRIX_COL_OOB,
  E_MATRIX_ROW_OOB,
  E_MATRIX_INVALID_COL_SIZE,
  E_MATRIX_INVALID_ROW_SIZE,
  E_PARMETIS_INVALID_NCON,
  E_PARMETIS_INVALID_NPARTS,
  E_PARMETIS_INVALID_WORK_ARRAY,
  E_PARMETIS_INVALID_WEIGHT_ARRAY,
  E_PARMETIS_INVALID_RESULT_ARRAY,
  E_PARMETIS_LIBRARY_ERROR,
  E_PARMETIS_UNDERSIZED_COMM,
  E_DISTGRAPH_UNFINALIZED,
  E_DISTGRAPH_FINALIZED,
  E_DISTGRAPH_NO_LOCAL_NODES,
  E_PARMETIS_INVALID_SUBDOMAIN_WEIGHT_ARRAYS,
  E_PARMETIS_INVALID_VERTEX_IMBALANCE_WEIGHT_ARRAYS,
  E_LINEARSOLVER_ROW_SIZE_UNSET,
  E_LINEARSOLVER_COL_SIZE_UNSET,
  E_LINEARSOLVER_INVALID_COMM,
  E_LINEARSOLVER_INVALID_VECTOR,
  E_LINEARSOLVER_INVALID_MATRIX,
  E_ARRAY_INCORRECT_SIZE,
  E_ARRAY_MISMATCH_SIZE,
  E_INVALID_INDEX,
  E_CMDLINE_ERR_ARG_INVALID_TYPE,
  E_CMDLINE_ERR_ARG_MISSING_FLAG,
  E_CMDLINE_ERR_ARG_INVALID_ARG,
  E_UNKNOWN_CELL_TYPE,
  E_NONPLANE_VERTEX,
  E_EUC_VEC_PARALLEL,
  E_MESH_INVALID_BOUNDARY_LABEL,
  E_MESH_INVALID_REGION_LABEL,
  E_MESH_INVALID_VERTEX_LABEL,
  E_MESH_INVALID_FACE_LABEL,
  E_MESH_INVALID_FACE,
  E_MESH_INVALID_CELL_LABEL,
  E_MESH_INVALID_VERTEX_COUNT,
  E_MESH_EXISTING_VERTEX,
  E_MESH_EXISTING_REGION,
  E_MESH_EXISTING_BOUNDARY,
  E_MESH_EXISTING_CELL,
  E_MESH_EXISTING_FACE,
  E_MESH_FACE_EDGE_EXISTS,
  E_MESH_UNMAPPED_BOUNDARY_FACE,
  E_CONFIG_MESH_SOURCE_TYPE_NOT_FOUND,
  E_CONFIG_INVALID_VALUE,
  E_CONFIG_INVALID_OPTION,
  E_CONFIG_OPT_NOT_FOUND,
  E_UNFINALIZED,
  E_FINALIZED,
  E_SUCCESS,
  E_FAILURE,
  E_NO_DATA,
  E_ERROR,
  E_NULL_PTR,
  E_NOT_NULL_PTR,
  E_INVALID_PARTITIONER_ID,
  E_MPI_DATATYPE_UNREGISTERED,
  E_MPI_DATATYPE_REGISTERED,
  E_MPI_ERR,
  E_INVALID_FILE_FORMAT,
  E_SPECIALISATION_ERROR,
  E_PARTITIONER_INVALID_WORK_ARRAY,
  E_PETSC_ERROR,
  E_NOT_IMPLEMENTED
};

extern const char* eStrings[];
}  // namespace cupcfd::error

#ifndef DEBUGGABLE_ERROR
#ifdef DEBUG
   // #define DEBUGGABLE_ERROR fprintf(stderr, "%s:%d\n\n", __FILE__, __LINE__);
   // fflush(stderr); fflush(stdout);
#define DEBUGGABLE_ERROR \
  { std::cout << __FILE__ << ":" << STRINGIZE(__LINE__) << std::endl; }
#else
#define DEBUGGABLE_ERROR
#endif
#endif

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#ifndef CHECK_ECODE
#ifdef DEBUG
#define CHECK_ECODE(E)                                   \
  {                                                      \
    if (E != cupcfd::error::E_SUCCESS) {                 \
      std::string msg(__FILE__);                         \
      msg += ":" + std::string(STRINGIZE(__LINE__)) +    \
             " - ERROR = " + cupcfd::error::eStrings[E]; \
      throw std::runtime_error(msg.c_str());             \
    }                                                    \
  }
#else
#define CHECK_ECODE(E)                           \
  {                                              \
    if (E != cupcfd::error::E_SUCCESS) return E; \
  }
#endif
#endif

#ifndef HARD_CHECK_ECODE
#ifdef DEBUG
#define HARD_CHECK_ECODE(E)                              \
  {                                                      \
    if (E != cupcfd::error::E_SUCCESS) {                 \
      std::string msg(__FILE__);                         \
      msg += ":" + std::string(STRINGIZE(__LINE__)) +    \
             " - ERROR = " + cupcfd::error::eStrings[E]; \
      throw std::runtime_error(msg.c_str());             \
    }                                                    \
  }
#else
#define HARD_CHECK_ECODE(E)                  \
  {                                          \
    if (E != cupcfd::error::E_SUCCESS) {     \
      std::string msg("ERROR = ");           \
      msg += cupcfd::error::eStrings[E];     \
      throw std::runtime_error(msg.c_str()); \
    }                                        \
  }
#endif
#endif

#ifndef DBG_HARD_CHECK_ECODE
#ifdef DEBUG
#define DBG_HARD_CHECK_ECODE(E)                          \
  {                                                      \
    if (E != cupcfd::error::E_SUCCESS) {                 \
      std::string msg(__FILE__);                         \
      msg += ":" + std::string(STRINGIZE(__LINE__)) +    \
             " - ERROR = " + cupcfd::error::eStrings[E]; \
      throw std::runtime_error(msg.c_str());             \
    }                                                    \
  }
#else
#define DBG_HARD_CHECK_ECODE(E)
#endif
#endif

#ifndef DBG_PRINT_BAD_ECODE
#ifdef DEBUG
#define DBG_PRINT_BAD_ECODE(E)                         \
  if (E != cupcfd::error::E_SUCCESS) {                 \
    std::string msg(__FILE__);                         \
    msg += ":" + std::string(STRINGIZE(__LINE__)) +    \
           " - ERROR = " + cupcfd::error::eStrings[E]; \
    std::cout << msg << std::endl;                     \
  }
#else
#define DBG_PRINT_BAD_ECODE(E)
#endif
#endif

#ifndef DBG_SAFE_VECTOR_LOOKUP
#ifdef DEBUG
#define DBG_SAFE_VECTOR_LOOKUP(V, I) ((V).at((I)))
#else
#define DBG_SAFE_VECTOR_LOOKUP(V, I) ((V)[(I)])
#endif
#endif

#endif
