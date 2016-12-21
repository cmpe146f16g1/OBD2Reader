#include <LPC17xx.h>
#include <iostream>
#include "printf_lib.h"
#include "io.hpp"
#include <ctype.h>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"
#include "ssp1.h"
#include "printf_lib.h"			//debug print
#include <stdint.h>				//Lab 6 sensors
#include "storage.hpp"			//Lab 7 FILE IO
#include "event_groups.h"		//Lab 7 event manager
#include "string.h"				//String handling
#include "rtc.h"				//Real Time Clock
#include "can.h"

#include "inttypes.h"			//For number to char conversion
#include "sjone_ssd1306.h"		//OLED Display Driver

using namespace std;

#define ELEMENTS 50
#define MAX_LINE 1024
#define CAN_BAUD_RATE		250		//if >100 use external oscillator
#define CODES 				1000	//3395 not enough space
string  obd2_codes[CODES][2];

//byte offset array for first 1000 lines
long ary[CODES] = {0,49,111, 159, 208, 254, 299, 346, 395, 444, 495, 575, 633, 686, 766, 824,
				   898, 972, 1046, 1120, 1172, 1252, 1310, 1364, 1444, 1502, 1573, 1645,
				   1716, 1788, 1840, 1896, 1953, 2002, 2055, 2109, 2162, 2218, 2275, 2348,
				   2413, 2478, 2530, 2586, 2643, 2705, 2780, 2841, 2903, 2947, 2999, 3055,
				   3112, 3159, 3206, 3253, 3305, 3361, 3418, 3465, 3512, 3559, 3611, 3667,
				   3724, 3778, 3837, 3886, 3930, 3995, 4040, 4095, 4144, 4194, 4252, 4305,
				   4362, 4420, 4474, 4532, 4592, 4645, 4702, 4760, 4814, 4872, 4932, 4974,
				   5017, 5061, 5109, 5161, 5214, 5257, 5300, 5346, 5410, 5460, 5511, 5578,
				   5628, 5692, 5740, 5789, 5840, 5913, 6000, 6071, 6143, 6217, 6266, 6329,
				   6376, 6424, 6474, 6527, 6594, 6645, 6697, 6751, 6817, 6897, 6961, 7026,
				   7093, 7161, 7221, 7275, 7329, 7384, 7440, 7503, 7564, 7618, 7672, 7727,
				   7783, 7846, 7907, 7961, 8015, 8070, 8126, 8189, 8250, 8304, 8358, 8413,
				   8469, 8532, 8593, 8647, 8701, 8756, 8812, 8875, 8936, 8990, 9044, 9099,
				   9155, 9218, 9279, 9316, 9347, 9378, 9415, 9446, 9477, 9527, 9583, 9631,
				   9680, 9732, 9784, 9834, 9885, 9938, 9990, 10048, 10098, 10149, 10202, 10254,
				   10312, 10362, 10413, 10466, 10514, 10568, 10608, 10649, 10698, 10733, 10781,
				   10829, 10877, 10925, 10973, 11021, 11069, 11117, 11165, 11214, 11263, 11312,
				   11352, 11392, 11434, 11485, 11517, 11563, 11597, 11663, 11743, 11807, 11872,
				   11939, 12005, 12085, 12149, 12214, 12281, 12325, 12363, 12402, 12449, 12482,
				   12536, 12596, 12642, 12689, 12743, 12803, 12849, 12896, 12948, 13006, 13050,
				   13095, 13147, 13205, 13249, 13294, 13374, 13460, 13532, 13605, 13686, 13766,
				   13852, 13924, 13997, 14078, 14116, 14155, 14199, 14237, 14276, 14320, 14358,
				   14397, 14441, 14479, 14518, 14562, 14600, 14639, 14683, 14721, 14760, 14804,
				   14842, 14881, 14925, 14963, 15002, 15046, 15084, 15123, 15167, 15206, 15246,
				   15291, 15330, 15370, 15415, 15454, 15494, 15539, 15587, 15621, 15655, 15689,
				   15723, 15757, 15791, 15825, 15859, 15893, 15928, 15963, 15998, 16064, 16136,
				   16200, 16267, 16334, 16407, 16472, 16538, 16612, 16662, 16717, 16765, 16814,
				   16871, 16926, 16987, 17040, 17094, 17150, 17201, 17258, 17307, 17357, 17409,
				   17467, 17527, 17587, 17647, 17707, 17767, 17827, 17887, 17947, 18007, 18067,
				   18127, 18187, 18247, 18311, 18374, 18450, 18508, 18568, 18632, 18695, 18771,
				   18829, 18876, 18929, 18976, 19031, 19092, 19145, 19199, 19255, 19304, 19363,
				   19419, 19471, 19529, 19582, 19636, 19689, 19743, 19792, 19853, 19928, 19996,
				   20067, 20142, 20210, 20281, 20346, 20411, 20469, 20528, 20584, 20642, 20701,
				   20759, 20818, 20874, 20932, 20991, 21045, 21108, 21177, 21259, 21334, 21412,
				   21487, 21555, 21626, 21708, 21778, 21854, 21922, 21991, 22062, 22131, 22175,
				   22225, 22267, 22310, 22355, 22399, 22449, 22491, 22534, 22579, 22621, 22669,
				   22703, 22738, 22781, 22830, 22885, 22926, 22968, 23018, 23066, 23114, 23162,
				   23210, 23249, 23300, 23339, 23384, 23429, 23482, 23520, 23570, 23621, 23671,
				   23731, 23789, 23841, 23894, 23952, 24016, 24072, 24129, 24175, 24232, 24295,
				   24350, 24406, 24464, 24497, 24527, 24552, 24578, 24621, 24665, 24712, 24756,
				   24802, 24848, 24904, 24952, 25001, 25047, 25091, 25144, 25183, 25243, 25283,
				   25368, 25394, 25442, 25490, 25534, 25587, 25641, 25708, 25752, 25809, 25853,
				   25899, 25951, 25996, 26054, 26100, 26166, 26224, 26274, 26325, 26378, 26442,
				   26512, 26574, 26637, 26702, 26755, 26814, 26865, 26919, 26969, 27015, 27059,
				   27095, 27134, 27185, 27230, 27281, 27324, 27370, 27397, 27426, 27455, 27484,
				   27513, 27542, 27577, 27626, 27689, 27736, 27785, 27836, 27880, 27937, 27978,
				   28021, 28066, 28101, 28186, 28218, 28290, 28326, 28361, 28446, 28478, 28550,
				   28586, 28621, 28669, 28701, 28735, 28771, 28806, 28854, 28886, 28920, 28956,
				   28991, 29039, 29071, 29105, 29141, 29187, 29233, 29276, 29321, 29368, 29392,
				   29420, 29448, 29476, 29504, 29583, 29629, 29661, 29694, 29735, 29787, 29833,
				   29897, 29953, 30012, 30046, 30105, 30148, 30193, 30240, 30290, 30358, 30422,
				   30471, 30538, 30591, 30645, 30707, 30743, 30775, 30803, 30832, 30873, 30902,
				   30933, 30963, 31011, 31076, 31123, 31159, 31195, 31244, 31293, 31348, 31387,
				   31430, 31474, 31490, 31526, 31566, 31607, 31643, 31683, 31724, 31768, 31830,
				   31878, 31927, 31985, 32061, 32123, 32186, 32257, 32315, 32391, 32453, 32516,
				   32587, 32629, 32675, 32722, 32755, 32792, 32830, 32866, 32920, 32960, 33001,
				   33047, 33097, 33148, 33180, 33230, 33266, 33303, 33337, 33375, 33414, 33472,
				   33548, 33610, 33673, 33744, 33802, 33878, 33940, 34003, 34074, 34103, 34150,
				   34183, 34217, 34259, 34303, 34345, 34388, 34424, 34478, 34518, 34559, 34608,
				   34637, 34675, 34702, 34728, 34766, 34824, 34883, 34920, 34968, 35002, 35037,
				   35103, 35156, 35195, 35235, 35283, 35315, 35379, 35432, 35471, 35511, 35600,
				   35652, 35690, 35729, 35776, 35816, 35900, 35959, 36004, 36050, 36136, 36196,
				   36242, 36289, 36335, 36392, 36435, 36479, 36519, 36569, 36619, 36670, 36723,
				   36770, 36827, 36884, 36942, 37002, 37032, 37087, 37134, 37176, 37229, 37268,
				   37308, 37345, 37396, 37440, 37470, 37501, 37540, 37593, 37634, 37661, 37689,
				   37725, 37782, 37850, 37904, 37959, 38016, 38084, 38138, 38193, 38250, 38318,
				   38372, 38427, 38484, 38527, 38571, 38628, 38671, 38715, 38772, 38815, 38859,
				   38916, 38959, 39003, 39060, 39103, 39147, 39205, 39281, 39343, 39406, 39477,
				   39535, 39611, 39673, 39736, 39807, 39864, 39907, 39951, 40022, 40102, 40152,
				   40203, 40232, 40266, 40375, 40497, 40552, 40586, 40620, 40696, 40771, 40799,
				   40904, 41011, 41086, 41151, 41205, 41260, 41304, 41374, 41443, 41508, 41562,
				   41606, 41672, 41717, 41758, 41801, 41852, 41902, 41952, 41995, 42037, 42088,
				   42126, 42176, 42228, 42278, 42308, 42361, 42394, 42443, 42494, 42547, 42599,
				   42651, 42690, 42729, 42763, 42800, 42852, 42904, 42941, 42983, 43027, 43072,
				   43115, 43140, 43166, 43196, 43219, 43243, 43271, 43299, 43314, 43359, 43405,
				   43446, 43495, 43547, 43598, 43647, 43671, 43702, 43732, 43772, 43795, 43818,
				   43846, 43870, 43906, 43933, 43959, 43997, 44032, 44063, 44115, 44167, 44219,
				   44271, 44323, 44375, 44420, 44473, 44534, 44592, 44633, 44684, 44732, 44781,
				   44832, 44847, 44861, 44910, 44952, 45002, 45049, 45105, 45142, 45182, 45228,
				   45273, 45309, 45347, 45397, 45447, 45486, 45525, 45562, 45599, 45645, 45691,
				   45733, 45771, 45807, 45844, 45890, 45930, 45969, 46004, 46035, 46075, 46117,
				   46156, 46203, 46246, 46288, 46331, 46373, 46411, 46449, 46487, 46525, 46566,
				   46607, 46648, 46689, 46730, 46771, 46812, 46853, 46894, 46932, 46974, 47014,
				   47054, 47094, 47134, 47174, 47214, 47254, 47294, 47344, 47395, 47438, 47464,
				   47497, 47534, 47584, 47635, 47690, 47733, 47775, 47831, 47887, 47926, 47989,
				   48040, 48091, 48134, 48152, 48206, 48236, 48265, 48293, 48321, 48348, 48374,
				   48419, 48459, 48490, 48528, 48578, 48628, 48653, 48697, 48744, 48789, 48842,
				   48895, 49000, 49046, 49092, 49130, 49169, 49228, 49308, 49358, 49408, 49458,
				   49508, 49560, 49608, 49660, 49712, 49756, 49802, 49823, 49844, 49865, 49912,
				   49962, 50020};

//--- search for obd2 code
//must modify for other types in the can_data_t union
void searchCode(can_data_t data)
{
    int i;
    char str[6];
    string obd;

    sprintf(str, "%llu", data.qword);
    string code = string(str);

    for(i=0; i< CODES; i++)
    {
         obd = obd2_codes[i][0].c_str();
        if(obd.compare(0,5,code) == 0)
        {
            printf("CODE: %s\t\t",obd2_codes[i][0].c_str());
            printf("MESSAGE: %s\n",obd2_codes[i][1].c_str());
        }
    }

}



//-- process obd2 codes
void processCodes(void)
{
	printf("in codes\n");
    char buffer[130];
    char *ptr;
    int  i, j;
    long *offset = ary;
    int looper;
    	i = 0; j= 0;
    	for(looper= 0; looper < 10; looper++)
        {

    		Storage::read("1:obdcodes.txt", buffer, sizeof(buffer), *offset);
            ptr = strtok(buffer, ":");
            obd2_codes[i][j] = ptr;
            //cout << obd2_codes[i][0];
            j++;
            ptr = strtok(NULL, "\n");
            obd2_codes[i][j] = ptr;
            //cout << obd2_codes[i][1];
            i++;j=0;
    		offset++;
        }

    	/*
        for(i=0; i< 10; i++)
        {
              printf("CODE: %s \tMESSAGE: %s\n", obd2_codes[i][0].c_str(), obd2_codes[i][1].c_str());

        }
		*/
}


//--- CAN task
class CAN_Task : public scheduler_task
{
	public:
		CAN_Task(uint8_t priority) : scheduler_task("CAN", 2048, priority){}
		can_t can_bus;//CAN BUS
		can_msg_t tx_msg1;
		can_msg_t rx_msg1;

		OLEDModule display;
		char *msgTransmit = "[CAN]: Transmit...";
		char *msgNoTransmit = "[CAN]: No Transmit";
		char *msgSent = "[CAN]: Msg sent :: ";
		char *msgReceive = "[CAN]: Msg received :: ";
		char *msgNoReceive = "[CAN]: No msg received";

		//------------------------------
		bool run(void *p)
		{
			printf("running\n");
			simpleCANTest();		//Message exchange
			vTaskDelay(1000);
			return true;
		}


		//------------------------------
		bool init(void)
		{
			processCodes();
			initCANBus();			//Prepare CAN Bus for operation
			//initGPIO();			//Prepare SJOne on board button
			return true;
		}

		//------------------------------
		void initCANBus()
		{
			uint16_t rxq_size = 16;
			uint16_t txq_size = 16;
			can_void_func_t x1 = NULL;
			can_void_func_t x2 = NULL;

			//Initialize bus, this leaves the bus disabled once done
			if(CAN_init(can1, CAN_BAUD_RATE, rxq_size, txq_size, x1, x2))
			{
				printf("CAN successfully initialized...\n");
			}
			else{
				printf("CAN could not be initialized...\n");
			}

			//Bypass filter to accept all incoming messages on CAN bus
			CAN_bypass_filter_accept_all_msgs();
			printf("CAN bypass filter enabled...\n");

			//Enable the CAN bus
			CAN_reset_bus(can1);

			if(!CAN_is_bus_off(can1))
			{
				printf("[CAN]: CAN bus is on.\n");
			}
			else
			{
				printf("[CAN]: CAN bus is off.\n");
			}
			return;
		}

		//------------------------------
		void initGPIO(void)
		{
			// Set up an on board button for later use
			// Set pin function							P1[9]
			LPC_PINCON->PINSEL2 &= ~(3 << 9);
			//Set direction for internal SW, input		P1[9]
			LPC_GPIO1->FIODIR &= ~(1 << 9);
			printf("[CAN]: GPIO interface initialized.\n");

		}

		//------------------------------
		void transmit(uint64_t hex_msg1)
		{
			char char_msg1[32];
			char dest[64];
			char src[32];

			display.displayString(0, 0, msgTransmit);
			display.showDisplay();

			//Create first test message
			tx_msg1.msg_id = 0x123;				//001 0010 0011
			tx_msg1.frame_fields.is_29bit = 0;
			//tx_msg1.frame_fields.data_len = 8;	//8 bytes
			tx_msg1.frame_fields.data_len = 1;		//1 byte (Also known as DLC)
			//tx_msg1.data.qword = 0x1122334455667788;
			//tx_msg1.data.qword = 0x0400334455664000;		//8 byte = 64 bit (data field)
			tx_msg1.data.qword = hex_msg1;			//1 byte (data field)

			/////////////////////////////////////////////////
			//TRANSMIT
			if(!CAN_tx(can1, &tx_msg1, portMAX_DELAY))
			{
				printf("[CAN]: TX Error");
				display.displayString(1, 0, msgNoTransmit);
				display.showDisplay();
			}
			else{
				printf("[CAN]: TX 1 Successful!\n");
				printf("[CAN]: Sent: %x\n\n", tx_msg1.data.bytes[0]);

				sprintf(char_msg1, "%" PRIu64, hex_msg1);
				strcpy(dest, msgSent);
				//strcpy(src, char_msg1);
				strcpy(src, "0x04");
				strcat(dest, src);
				//display.displayString(1, 0, dest);
				display.displayString(1, 0, dest);
				display.showDisplay();
			}
		}

		//------------------------------
		void receive(void)
		{
			//string data;
			can_data_t data;
			//RECEIVE
			if(!CAN_rx(can1, &rx_msg1, 1000))
			{
				printf("[CAN]: Nothing Received\n");
				display.displayString(4, 0, msgNoReceive);
				display.showDisplay();
			}
			else{
				//message was received
				data = rx_msg1.data;
				searchCode(data);
				printf("[CAN]: Received: %x\n", rx_msg1.data.bytes[0]);
			}
		}


		//------------------------------
		void simpleCANTest(void)
		{
			uint64_t hex_msg1 = 0x04;

			printf("\n[CAN]: Sending a simple message...\n");
			//Need to re-enable CAN bus
			CAN_reset_bus(can1);
			if(!CAN_is_bus_off(can1))
			{
					//send message
					transmit(hex_msg1);
					//Obtain results
					receive();
					delay_ms(5000);
					display.clearDisplay();
			}
			else{

				printf("[CAN]: CAN Bus is off\n");
			}

		}
};

int main(void)
{
		scheduler_add_task(new terminalTask(PRIORITY_HIGH));
		scheduler_add_task(new CAN_Task(PRIORITY_HIGH));
		scheduler_start();
		return 0;
}
