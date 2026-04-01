#ifndef ROUTES_H
#define ROUTES_H

#include "main.h"

/* ================================================================
 *  SPEEDS  (0.0 – 1.0)
 * ================================================================ */
#define ROUTE_SPEED_NORMAL          0.75f   // Matched to your AUTOMODE_SPEED
#define ROUTE_SPEED_SLOW            0.50f
#define ROUTE_SPEED_TURN            0.60f

/* ================================================================
 *  TIMING (ms) — tune to your surface and speed
 * ================================================================ */
#define ROUTE_FWD_SEGMENT_MS        1500U   // one open-loop forward leg
#define ROUTE_TURN_90_MS            800U    // 90° pivot
#define ROUTE_TURN_120_MS           1050U   // 120° pivot (triangle)
#define ROUTE_TURN_45_MS            400U    // 45° pivot
#define ROUTE_SETTLE_MS             150U    // pause between moves

/* ================================================================
 *  TOF COLLISION AVOIDANCE
 * ================================================================ */
#define ROUTE_TOF_MIN_DIST_MM       100.0f  // Matches STOP_DISTANCE_MM
#define ROUTE_TOF_POLL_MS           50U     // ToF sample interval in ms

/* ================================================================
 *  ── OPEN-LOOP GEOMETRIC ROUTES ──
 * ================================================================ */
void Route_Square(void);
void Route_Triangle(void);
void Route_Zigzag(void);
void Route_Spiral(void);
void Route_LShape(void);
void Route_Slalom(void);

#endif /* ROUTES_H */