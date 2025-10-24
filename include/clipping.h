#ifndef CLIPPING_H
#define CLIPPING_H

enum Outcode {
    INSIDE = 0, // 0000
    LEFT   = 1, // 0001
    RIGHT  = 2, // 0010
    BOTTOM = 4, // 0100
    TOP    = 8  // 1000
};

Outcode computeOutcode(double x, double y);

bool clipparReta(double& x1, double& y1, double& x2, double& y2);

#endif // CLIPPING_H
