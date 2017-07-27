//-------- MQTT ------------
#define ORG "2kcheh"
#define DEVICE_TYPE "yun"
#define DEVICE_ID "B4218AF04AD4" // MAC ADDRESS
#define TOKEN "uLefEe232fyVvGmaDl" // TOKEN DO BLUEMIX

//-------- GEOLOCATION ------
float lat;
float lng;

//------------------ MQ5 -----
#define         MQ5PIN                       (0)      //define which analog input channel you are going to use
#define         RL_VALUE_MQ5                 (1)      //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ5      (6.455)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in
//normal operation
/**********************Application Related Macros**********************************/
#define         GAS_HYDROGEN                 (0)
#define         GAS_LPG                      (1)
#define         GAS_METHANE                  (2)
#define         GAS_CARBON_MONOXIDE          (3)
#define         GAS_ALCOHOL                  (4)
#define         accuracy                     (0)    //for linearcurves
//#define         accuracy                   (1)    //for nonlinearcurves, un comment this line and comment the above line if calculations 
                                                    //are to be done using non linear curve equations
/*****************************Globals************************************************/
float Ro = 10;                                                                                           //Ro is initialized to 10 kilo ohms
