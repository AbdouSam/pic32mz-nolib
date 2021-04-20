#include "dio.h"
/**
 * @brief Turns on a pin.
 *
 * Turns on a pin if it is an output.
 *
 * @param out The pin to turn on.
 *
 * @return OK on success, an error code otherwise.
 */

int dio_turnon(int out);

/**
 * @brief Turns off a pin.
 *
 * Turns off a pin if it is an output.
 *
 * @param out The pin to turn off.
 *
 * @return OK on success, an error code otherwise.
 *
 */

int dio_turnoff(int out);

/**
 * @brief Toggles a pin.
 *
 * Toggles a pin if it is an output.
 *
 * @param out The pin to toggle.
 *
 * @return OK on success, an error code otherwise.
 */

int dio_toggle(int out);

/**
 * @brief Write to a pin.
 *
 * Write the value state to the given pin. The pin must be an output.
 *
 * @param out The pin to write to.
 * @param state The state to write.
 *
 *
 * @return OK on success, an error code otherwise.
 */

int dio_write(int out, bool state);

/**
 * @brief Reads a pin.
 *
 * Reads a pin.
 *
 * @param in The pin to read.
 *
 * @return True if input set, false otherwise.
 *
 */

bool dio_read(int in);