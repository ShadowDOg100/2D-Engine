#ifndef AICOREMESSAGE
#define AICOREMESSAGE
 

 
#define AI_T_Trigger                    3000            //Defines a trigger message from the AI Core
#define AI_Q_EnableTrigger              0               //Defines a qualifier for AI_T_Trigger to redraw
 
                                                                                        // P1: Relevant control classification
 
                                                                                        // P2: Value to set to 
 
#define AI_Q_ReEnableTrigger			1               //Defines a qualifier for AI_T_Trigger to process
 
                                                                                        // P1: Relevant control classification
 
                                                                                        // P2; Value to reset to
#define AI_Q_TriggerTripped				2				// Trigger is tripped
																						// P1.i: Trigger id tripped
																						// P2.i: User Information
 
#define AI_T_Projectiles                3001            //Defines a message from the AI Core 
#define AI_T_ProjectilesTrigger			3002            //Defines a message from the AI Core to the Main Core
 

 
#endif 
 
 
