#ifndef MOTOR_H
#define MOTOR_H

void GoRight(float speed);
void GoForward(float speed);

void GoLeft(float speed);
void GoBackward(float speed);

void GoTRDiagonal(float speed);
void GoTLDiagonal(float speed);
void GoBRDiagonal(float speed);
void GoBLDiagonal(float speed);

void StopMotors(void);

void Spin180(void);

#endif