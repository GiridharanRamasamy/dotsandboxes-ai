#include "box.h"

/**
 * Constructs a new box
 */
Box::Box() {
    left = false;
    right = false;
    top = false;
    bottom = false;
}

/**
 * Fills left
 *@return true if this box is complete
 */
bool Box::fillLeft() {
    left = true;
    return(bottom&&top&&left&&right);
}

/**
 * Fills right
 *@return true if this box is complete
 */
bool Box::fillRight() {
    right = true;
    return(bottom&&top&&left&&right);
}

/**
 * Fills top
 *@return true if this box is complete
 */
bool Box::fillTop() {
    top = true;
    return(bottom&&top&&left&&right);
}

/**
 * Fills bottom
 *@return true if this box is complete
 */
bool Box::fillBottom() {
    bottom = true;
    return(bottom&&top&&left&&right);
}
