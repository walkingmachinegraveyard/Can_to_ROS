#ifndef LOWLEVEL_CAN_DEFINITION_H
#define LOWLEVEL_CAN_DEFINITION_H

#ifdef __cplusplus
extern "C" {
#endif

/* add guard for the case that no CAN device is defined */
#if CAN_NUMOF

typedef enum {
#if CAN_0_EN
    CAN_0 = 0,          /**< CAN device 0 */
#endif
#if CAN_1_EN
    CAN_1,              /**< CAN device 1 */
#endif
#if CAN_2_EN
    CAN_2,              /**< CAN device 2 */
#endif
#if CAN_3_EN
    CAN_3,              /**< CAN device 3 */
#endif
} can_t;

#else
#define CAN_NUMOF 0
#endif /* CAN_NUMOF */

#ifdef __cplusplus
}
#endif


#endif /* LOWLEVEL_CAN_DEFINITION_H */
