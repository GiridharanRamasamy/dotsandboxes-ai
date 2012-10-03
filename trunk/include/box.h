#ifndef BOX_H
#define BOX_H

/**
 * The object is to complete these
 */
class Box
{
    public:
        /**
         * Constructs a new box
         */
        Box();

        /**
         * Fills left
         *@return true if this box is complete
         */
        bool fillLeft();

        /**
         * Fills right
         *@return true if this box is complete
         */
        bool fillRight();

        /**
         * Fills top
         *@return true if this box is complete
         */
        bool fillTop();

        /**
         * Fills bottom
         *@return true if this box is complete
         */
        bool fillBottom();
    private:
        bool left; /**< whether or not left has been filled */
        bool right; /**< whether or not left has been filled */
        bool top; /**< whether or not left has been filled */
        bool bottom; /**< whether or not left has been filled */
};

#endif // BOX_H
