#ifndef ROUTES_H
#define ROUTES_H

#include "main.h"

/* ── Speeds (0.0 – 1.0) ─────────────────────────────────────────── */
#define ROUTE_SPEED_NORMAL      0.6f
#define ROUTE_SPEED_SLOW        0.4f
#define ROUTE_SPEED_TURN        0.5f

/* ── Timing constants (ms) — tune to your surface/speed ─────────── */
#define ROUTE_FWD_SEGMENT_MS    1500U   // one forward leg
#define ROUTE_TURN_90_MS        800U    // 90° pivot
#define ROUTE_TURN_120_MS       1050U   // 120° pivot (triangle)
#define ROUTE_TURN_45_MS        400U    // 45° pivot
#define ROUTE_SETTLE_MS         100U    // brief pause between moves

/* ── ToF collision avoidance ────────────────────────────────────── */
#define ROUTE_TOF_MIN_DIST_MM   150U    // stop if obstacle closer than this
#define ROUTE_TOF_POLL_MS       50U     // how often to sample ToF during movement

/* ── Route functions ────────────────────────────────────────────── */
void Route_Square(void);
void Route_Triangle(void);
void Route_Zigzag(void);
void Route_Spiral(void);
void Route_LShape(void);
void Route_Slalom(void);

#endif /* ROUTES_H */