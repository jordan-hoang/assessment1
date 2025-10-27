#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "IQueryOperator.h" // Defines the IQueryOperator interface
#include "json.hpp"         // Defines the nlohmann::json type
#include <memory>


namespace QueryBuilder {
  /**
   * @brief Parses the root of a JSON query object and recursively builds the
   * IQueryOperator tree (Composite Pattern).
   * This function serves as the entry point, calling the internal recursive logic.
   * @param json_query_root The JSON object representing the beginning of the "QUERY" part.
   * @return std::unique_ptr<IQueryOperator> A unique_ptr to the root of the constructed operator tree.
   */
  std::unique_ptr<IQueryOperator> parse_query_root(const nlohmann::json& json_query_root);
}

#endif //QUERYBUILDER_H
