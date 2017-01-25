#ifndef LOWLEVEL_CAN_DEFINITION_H
#define LOWLEVEL_CAN_DEFINITION_H

/**
 * @file lowLevel_CAN_definition.h
 * @brief Universal low-level declaration
 * @author Alexandre Salconi-Denis
 * @date 25 september 2017
 * @version 0.1
 */

#ifdef __cplusplus
extern "C" {
#endif

// Add guard for the case that no CAN device is defined 
#if CAN_NUMOF

/** 
 *  @enum can_t
 *  @brief Declare how many CAN exist on the platform
 */    
typedef enum {
#if CAN_0_EN
    CAN_0 = 0,          /*!< CAN device 0 */
#endif
#if CAN_1_EN
    CAN_1,              /*!< CAN device 1 */
#endif
#if CAN_2_EN
    CAN_2,              /*!< CAN device 2 */
#endif
#if CAN_3_EN
    CAN_3,              /*!< CAN device 3 */
#endif
} can_t;

#else
/**
 * @def CAN_NUMOF
 * Definition qui retourne le nombre de CAN disponible dans can_t
 * @note Devrait etre declarer au prealable si il y a des port CAN de disponible
 */
#define CAN_NUMOF 0
#endif /* CAN_NUMOF */

#ifdef __cplusplus
}
#endif


#endif /* LOWLEVEL_CAN_DEFINITION_H */
