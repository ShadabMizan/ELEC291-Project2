#include "routes.h"
#include "motor.h"
#include "tof.h"    // ← verify TOF_GetDistance() matches your actual API

/* ================================================================
 *  INTERNAL HELPERS
 * ================================================================ */

/**
 * @brief Move forward for duration_ms, polling ToF every ROUTE_TOF_POLL_MS.
 *        Stops immediately and returns 0 if an obstacle is detected.
 * @return 1 if segment completed, 0 if aborted by obstacle
 */
static uint8_t SafeForward(float speed, uint32_t duration_ms)
{
    uint32_t elapsed = 0;
    GoForward(speed);
    while (elapsed < duration_ms) {
        HAL_Delay(ROUTE_TOF_POLL_MS);
        elapsed += ROUTE_TOF_POLL_MS;
        if (TOF_GetDistance() < ROUTE_TOF_MIN_DIST_MM) {
            StopMotors();
            return 0;
        }
    }
    StopMotors();
    return 1;
}

/**
 * @brief Move backward for duration_ms (no ToF check — reversing away from obstacle).
 */
static void SafeBackward(float speed, uint32_t duration_ms)
{
    GoBackward(speed);
    HAL_Delay(duration_ms);
    StopMotors();
}

/**
 * @brief Pivot right for turn_ms then stop.
 */
static void PivotRight(uint32_t turn_ms)
{
    GoRight(ROUTE_SPEED_TURN);
    HAL_Delay(turn_ms);
    StopMotors();
    HAL_Delay(ROUTE_SETTLE_MS);
}

/**
 * @brief Pivot left for turn_ms then stop.
 */
static void PivotLeft(uint32_t turn_ms)
{
    GoLeft(ROUTE_SPEED_TURN);
    HAL_Delay(turn_ms);
    StopMotors();
    HAL_Delay(ROUTE_SETTLE_MS);
}

/* ================================================================
 *  ROUTES
 * ================================================================ */

/**
 * @brief Square — 4 equal forward legs with 90° right pivots.
 *
 *        →  ↓
 *        ↑  ←
 */
void Route_Square(void)
{
    for (int i = 0; i < 4; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        PivotRight(ROUTE_TURN_90_MS);
    }
}

/**
 * @brief Triangle — 3 equal forward legs with 120° right pivots.
 *
 *          △
 */
void Route_Triangle(void)
{
    for (int i = 0; i < 3; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        PivotRight(ROUTE_TURN_120_MS);
    }
}

/**
 * @brief Zigzag — forward legs with alternating 45° right/left pivots.
 *        Produces a sawtooth path across the floor.
 *
 *        /\/\/\
 */
void Route_Zigzag(void)
{
    for (int i = 0; i < 6; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS)) return;
        /* Alternate pivot direction each leg */
        if (i % 2 == 0) {
            PivotRight(ROUTE_TURN_45_MS);
        } else {
            /* Double-left to cross back over centerline */
            PivotLeft(ROUTE_TURN_45_MS * 2);
        }
    }
    /* Return to original heading on exit */
    PivotRight(ROUTE_TURN_45_MS);
}

/**
 * @brief Spiral — outward square spiral; each pair of legs gets longer.
 *
 *        ┌──────┐
 *        │ ┌──┐ │
 *        │ └┘ │ │
 *        └────┘ │
 *               ↓ (exits outward)
 */
void Route_Spiral(void)
{
    uint32_t seg_ms = ROUTE_FWD_SEGMENT_MS;

    for (int i = 0; i < 8; i++) {
        if (!SafeForward(ROUTE_SPEED_NORMAL, seg_ms)) return;
        PivotRight(ROUTE_TURN_90_MS);
        /* Every 2 turns, increase segment length to spiral outward */
        if (i % 2 == 1) seg_ms += 500U;
    }
}

/**
 * @brief L-Shape — long forward leg, 90° left, shorter forward leg.
 *
 *        ──┐
 *          ↓
 */
void Route_LShape(void)
{
    /* Long horizontal arm */
    if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS * 2)) return;
    PivotLeft(ROUTE_TURN_90_MS);
    /* Short vertical arm */
    SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS);
}

/**
 * @brief Slalom — weaves left and right around imaginary gates.
 *        Forward → veer right → re-centre → forward → veer left → re-centre → repeat
 *
 *        ~S~S~S~
 */
void Route_Slalom(void)
{
    for (int i = 0; i < 4; i++) {
        if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS)) return;

        if (i % 2 == 0) {
            /* Veer right */
            PivotRight(ROUTE_TURN_45_MS);
            if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS / 2)) return;
            /* Re-centre */
            PivotLeft(ROUTE_TURN_45_MS);
        } else {
            /* Veer left */
            PivotLeft(ROUTE_TURN_45_MS);
            if (!SafeForward(ROUTE_SPEED_SLOW, ROUTE_FWD_SEGMENT_MS / 2)) return;
            /* Re-centre */
            PivotRight(ROUTE_TURN_45_MS);
        }
    }
}