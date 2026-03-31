#include "routes.h"
#include "motor.h"
#include "tof.h"      // TOF_GetDistance() -> uint32_t mm
#include "vsensor.h"  // VSENSOR_GetLeft() / VSENSOR_GetRight() -> uint16_t ADC counts

/* ================================================================
 *  PATH LOOKUP TABLE  (project spec Section 9, verbatim)
 *
 *  path_table[path][intersection]
 *  path  : 0 = Path 1  |  1 = Path 2  |  2 = Path 3
 *  index : 0–7         = Intersections 1–8
 * ================================================================ */
static const RouteAction_t path_table[ROUTE_NUM_PATHS][ROUTE_NUM_INTERSECTIONS] = {
    /* Path 1 */ { ROUTE_ACT_FORWARD, ROUTE_ACT_LEFT,    ROUTE_ACT_LEFT,    ROUTE_ACT_FORWARD,
                   ROUTE_ACT_RIGHT,   ROUTE_ACT_LEFT,    ROUTE_ACT_RIGHT,   ROUTE_ACT_STOP    },

    /* Path 2 */ { ROUTE_ACT_LEFT,    ROUTE_ACT_RIGHT,   ROUTE_ACT_LEFT,    ROUTE_ACT_RIGHT,
                   ROUTE_ACT_FORWARD, ROUTE_ACT_FORWARD, ROUTE_ACT_STOP,    ROUTE_ACT_STOP    },

    /* Path 3 */ { ROUTE_ACT_RIGHT,   ROUTE_ACT_FORWARD, ROUTE_ACT_RIGHT,   ROUTE_ACT_LEFT,
                   ROUTE_ACT_RIGHT,   ROUTE_ACT_LEFT,    ROUTE_ACT_FORWARD, ROUTE_ACT_STOP    },
};

/* ================================================================
 *  SHARED INTERNAL HELPERS
 * ================================================================ */

static uint8_t IsClear(void)
{
    return (TOF_GetDistance() >= ROUTE_TOF_MIN_DIST_MM) ? 1U : 0U;
}

/* ── Geometric route helpers ──────────────────────────────────── */

/**
 * @brief Drive forward for duration_ms, polling ToF every ROUTE_TOF_POLL_MS.
 * @return 1 = completed,  0 = aborted by obstacle
 */
static uint8_t SafeForward(float speed, uint32_t duration_ms)
{
    uint32_t elapsed = 0;
    GoForward(speed);
    while (elapsed < duration_ms) {
        HAL_Delay(ROUTE_TOF_POLL_MS);
        elapsed += ROUTE_TOF_POLL_MS;
        if (!IsClear()) {
            StopMotors();
            return 0;
        }
    }
    StopMotors();
    return 1;
}

static void PivotRight(uint32_t turn_ms)
{
    GoRight(ROUTE_SPEED_TURN);
    HAL_Delay(turn_ms);
    StopMotors();
    HAL_Delay(ROUTE_SETTLE_MS);
}

static void PivotLeft(uint32_t turn_ms)
{
    GoLeft(ROUTE_SPEED_TURN);
    HAL_Delay(turn_ms);
    StopMotors();
    HAL_Delay(ROUTE_SETTLE_MS);
}

/* ── Wire-following helpers ───────────────────────────────────── */

static uint8_t IsIntersection(void)
{
    uint16_t left  = VSENSOR_GetLeft();
    uint16_t right = VSENSOR_GetRight();
    return (left > ROUTE_INTERSECTION_THRESHOLD && right > ROUTE_INTERSECTION_THRESHOLD) ? 1U : 0U;
}

static void SteerOnWire(void)
{
    int32_t error = (int32_t)VSENSOR_GetLeft() - (int32_t)VSENSOR_GetRight();

    if (error > (int32_t)ROUTE_STEER_DEADBAND) {
        GoRight(ROUTE_SPEED_FOLLOW);    // drifted left, correct right
    } else if (error < -(int32_t)ROUTE_STEER_DEADBAND) {
        GoLeft(ROUTE_SPEED_FOLLOW);     // drifted right, correct left
    } else {
        GoForward(ROUTE_SPEED_FOLLOW);  // centred
    }
}

/* ================================================================
 *  OPEN-LOOP GEOMETRIC ROUTES
 * ================================================================ */

/**
 *  Square — 4 equal forward legs, 90° right pivots
 *
 *       →  ↓
 *       ↑  ←
 */
void Route_Square(void)
{
    for (int i = 0; i < 4; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        PivotRight(ROUTE_TURN_90_MS);
    }
}

/**
 *  Triangle — 3 equal forward legs, 120° right pivots
 *
 *       △
 */
void Route_Triangle(void)
{
    for (int i = 0; i < 3; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        PivotRight(ROUTE_TURN_120_MS);
    }
}

/**
 *  Zigzag — sawtooth path with alternating 45° pivots
 *
 *       /\/\/\
 */
void Route_Zigzag(void)
{
    for (int i = 0; i < 6; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        if (i % 2 == 0) {
            PivotRight(ROUTE_TURN_45_MS);
        } else {
            PivotLeft(ROUTE_TURN_45_MS * 2);  // cross back over centreline
        }
    }
    PivotRight(ROUTE_TURN_45_MS); // restore original heading
}

/**
 *  Spiral — outward square spiral, each pair of legs grows longer
 *
 *       ┌──────┐
 *       │ ┌──┐ │
 *       │ └┘ │ │
 *       └────┘ ↓
 */
void Route_Spiral(void)
{
    uint32_t seg_ms = ROUTE_FWD_SEGMENT_MS;
    for (int i = 0; i < 8; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, seg_ms)) return;
        PivotRight(ROUTE_TURN_90_MS);
        if (i % 2 == 1) seg_ms += 500U;  // lengthen every 2 legs
    }
}

/**
 *  L-Shape — long forward arm, 90° left, shorter forward arm
 *
 *       ──┐
 *         ↓
 */
void Route_LShape(void)
{
    if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS * 2)) return;
    PivotLeft(ROUTE_TURN_90_MS);
    SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS);
}

/**
 *  Slalom — weaves around imaginary gates, alternating left/right
 *
 *       ~S~S~S~
 */
void Route_Slalom(void)
{
    for (int i = 0; i < 4; i++) {
        if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS)) return;

        if (i % 2 == 0) {
            PivotRight(ROUTE_TURN_45_MS);
            if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS / 2)) return;
            PivotLeft(ROUTE_TURN_45_MS);
        } else {
            PivotLeft(ROUTE_TURN_45_MS);
            if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS / 2)) return;
            PivotRight(ROUTE_TURN_45_MS);
        }
    }
}

/* ================================================================
 *  GUIDE WIRE ROUTES
 * ================================================================ */

/**
 * @brief Follow the guide wire, steering continuously, until an
 *        intersection or obstacle is detected.
 * @return 1 = intersection reached,  0 = obstacle stopped route
 */
uint8_t Route_FollowWireUntilIntersection(void)
{
    while (1) {
        if (!IsClear()) {
            StopMotors();
            return 0;
        }
        if (IsIntersection()) {
            StopMotors();
            return 1;
        }
        SteerOnWire();
        HAL_Delay(ROUTE_TOF_POLL_MS);
    }
}

/**
 * @brief Execute one intersection action then clear the intersection zone.
 */
void Route_ExecuteAction(RouteAction_t action)
{
    switch (action) {
        case ROUTE_ACT_FORWARD:
            GoForward(ROUTE_SPEED_FOLLOW);
            HAL_Delay(ROUTE_INTERSECTION_HOLD_MS);
            StopMotors();
            break;

        case ROUTE_ACT_LEFT:
            GoLeft(ROUTE_SPEED_TURN);
            HAL_Delay(ROUTE_TURN_90_MS);
            StopMotors();
            break;

        case ROUTE_ACT_RIGHT:
            GoRight(ROUTE_SPEED_TURN);
            HAL_Delay(ROUTE_TURN_90_MS);
            StopMotors();
            break;

        case ROUTE_ACT_STOP:
        default:
            StopMotors();
            return; // skip settle delay on stop
    }

    HAL_Delay(ROUTE_SETTLE_MS);
}

/**
 * @brief  Run a complete pre-configured guide wire path.
 *         Follows the figure-8 wire and executes the spec-mandated
 *         decision at each of up to 8 intersections.
 * @param  path  0 = Path 1  |  1 = Path 2  |  2 = Path 3
 */
void Route_Run(uint8_t path)
{
    if (path >= ROUTE_NUM_PATHS) return;

    for (uint8_t i = 0; i < ROUTE_NUM_INTERSECTIONS; i++) {
        if (!Route_FollowWireUntilIntersection()) return;  // obstacle — abort

        RouteAction_t action = path_table[path][i];
        Route_ExecuteAction(action);

        if (action == ROUTE_ACT_STOP) return;
    }

    StopMotors();
}