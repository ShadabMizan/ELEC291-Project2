#include "routes.h"
#include "motor.h"
#include "tof.h"      

/* ================================================================
 *  INTERNAL HELPERS
 * ================================================================ */

/**
 * @brief Checks ToF sensor. 
 * @return 1 = Clear to drive, 0 = Obstacle detected (stop)
 */
static uint8_t IsClear(void)
{
    float dist = GetRange_mm();
    // Matches logic from automode.c: stop if <= 100mm and > 0
    if (dist <= ROUTE_TOF_MIN_DIST_MM && dist > 0.0f) {
        return 0U; 
    }
    return 1U;
}

/**
 * @brief Drive forward for duration_ms, polling ToF every ROUTE_TOF_POLL_MS.
 * @return 1 = completed, 0 = aborted by obstacle
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
            return 0; // Abort route
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

/* ================================================================
 *  OPEN-LOOP GEOMETRIC ROUTES
 * ================================================================ */

/**
 *  Square — 4 equal forward legs, 90° right pivots
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
 */
void Route_LShape(void)
{
    if (!SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS * 2)) return;
    PivotLeft(ROUTE_TURN_90_MS);
    SafeForward(ROUTE_SPEED_NORMAL, ROUTE_FWD_SEGMENT_MS);
}

/**
 *  Slalom — weaves around imaginary gates, alternating left/right
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