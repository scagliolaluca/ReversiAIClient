#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#include "currentstate.h"
#include "gamedetails.h"
#include "logging.h"

namespace Map {
/** @brief loads map from path
 * @param path path to map
 * @return bool
 * @retval true loding the map succeeded
 * @retval false loding the map failed
 */
bool loadMap(std::string path);

/** @brief loads map from stringstream
 * @param mapStream steam that includes the whole map
 * @return bool
 * @retval true loding the map succeeded
 * @retval false loding the map failed
 */
bool loadMap(std::stringstream& mapStream);
}  // namespace Map

#endif  // MAP_H
