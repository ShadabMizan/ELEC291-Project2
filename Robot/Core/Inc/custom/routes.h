#ifndef ROUTES_H
#define ROUTES_H

#include "main.h"

/* ================================================================
 *  SPEEDS  (0.0 – 1.0)
 * ================================================================ */
#define ROUTE_SPEED_NORMAL          0.6f
#define ROUTE_SPEED_SLOW            0.4f
#define ROUTE_SPEED_TURN            0.5f
#define ROUTE_SPEED_FOLLOW          0.5f

/* ================================================================
 *  TIMING (ms) — tune to your surface and speed
 * ================================================================ */
#define ROUTE_FWD_SEGMENT_MS        1500U   // one open-loop forward leg
#define ROUTE_TURN_90_MS            800U    // 90° pivot
#define ROUTE_TURN_120_MS           1050U   // 120° pivot (triangle)
#define ROUTE_TURN_45_MS            400U    // 45° pivot
#define ROUTE_SETTLE_MS             150U    // pause between moves
#define ROUTE_INTERSECTION_HOLD_MS  300U    // distance to clear intersection before resuming follow

/* ================================================================
 *  TOF COLLISION AVOIDANCE
 * ================================================================ */
#define ROUTE_TOF_MIN_DIST_MM       150U    // stop if obstacle closer than this (mm)
#define ROUTE_TOF_POLL_MS           50U     // ToF sample interval

/* ================================================================
 *  WIRE-FOLLOWING (vsensor)
 * ================================================================ */
#define ROUTE_STEER_DEADBAND        50U     // ADC count balance tolerance — tune this
#define ROUTE_INTERSECTION_THRESHOLD 400U   // both sensors above this = intersection — tune this

/* ================================================================
 *  GUIDE WIRE PATHS  (from project spec Section 9)
 * ================================================================ */
#define ROUTE_NUM_PATHS             3
#define ROUTE_NUM_INTERSECTIONS     8

typedef enum {
    ROUTE_ACT_FORWARD = 0,
    ROUTE_ACT_LEFT,
    ROUTE_ACT_RIGHT,
    ROUTE_ACT_STOP
} RouteAction_t;

/* ================================================================
 *  ── OPEN-LOOP GEOMETRIC ROUTES ──
 *  These run without a guide wire (e.g. for manual demo / testing)
 * ================================================================ */
void Route_Square(void);
void Route_Triangle(void);
void Route_Zigzag(void);
void Route_Spiral(void);
void Route_LShape(void);
void Route_Slalom(void);

/* ================================================================
 *  ── GUIDE WIRE ROUTES ──
 *  These follow the magnetic field on the figure-8 wire layout and
 *  execute the pre-programmed intersection decisions per the spec.
 *  path: 0 = Path 1, 1 = Path 2, 2 = Path 3
 * ================================================================ */
void Route_Run(uint8_t path);

/* Lower-level helpers (exposed for automode.c if needed) */
uint8_t Route_FollowWireUntilIntersection(void);
void    Route_ExecuteAction(RouteAction_t action);

#endif /* ROUTES_H */