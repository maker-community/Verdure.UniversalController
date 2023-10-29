// controller_keys.h

#ifndef _CONTROLLER_KEYS_h              // 防止重复包含
#define _CONTROLLER_KEYS_h

#if defined(ARDUINO) && ARDUINO >= 100  // arduino的版本>= 100？
	#include "arduino.h"                // 这个头文件包含了 Arduino 编程所需的基本定义和函数声明。
#else
	#include "WProgram.h"               // 在旧版本的 Arduino 环境中，"WProgram.h" 是主要的头文件，用于定义基本函数和变量。
#endif

/*  -----------此处为新板子的设置-------------------
#define PIN_LX 2          // 左摇杆X
#define PIN_LY 1		  // 左摇杆Y
#define PIN_L_KNOB 3	  // 左旋钮
#define PIN_L_UP 46		  // 左侧上按键
#define PIN_L_DOWN 45     // 左侧下按键
				  
#define PIN_RX 16         // 右摇杆X
#define PIN_RY 15		  // 右摇杆Y
#define PIN_R_KNOB 14	  // 右旋钮
#define PIN_R_UP 44		  // 右侧上按键	  
#define PIN_R_DOWN 39     // 右侧下按键
						  
#define MCP_PIN_L1 7      // 左侧拨杆1 与mcp2307连接的引脚     左1---左2---右1---右2
#define MCP_PIN_L2 4      // 左侧拨杆2 与mcp2307连接的引脚
#define MCP_PIN_R1 5      // 右侧拨杆1 与mcp2307连接的引脚
#define MCP_PIN_R1 6      // 右侧拨杆2 与mcp2307连接的引脚
						  
#define MCP_PIN_UP 8	  // 功能键 上
#define MCP_PIN_DOWN 11	  // 功能键 下
#define MCP_PIN_LEFT 9	  // 功能键 左
#define MCP_PIN_RIGHT 10  // 功能键 右
#define MCP_PIN_O 12	  // 功能键 O
#define MCP_PIN_X 13	  // 功能键 X
#define MCP_PIN_A 14	  // 功能键 A
#define MCP_PIN_B 15	  // 功能键 B
*/


// --------------------- 以下为画错的板子的临时设置，新板子到手后删除----------------------------------
#define PIN_LX 2          // 左摇杆X
#define PIN_LY 1		  // 左摇杆Y
#define PIN_L_KNOB 3	  // 左旋钮
#define PIN_L_UP 46		  // 左侧上按键
#define PIN_L_DOWN 39    // 左侧下按键

#define PIN_RX 16         // 右摇杆X
#define PIN_RY 15		  // 右摇杆Y
#define PIN_R_KNOB 14	  // 右旋钮
#define PIN_R_UP 45		  // 右侧上按键
#define PIN_R_DOWN 44     // 右侧下按键

#define PIN_BOARD_L 0     // 板子左侧按键
#define PIN_BOARD_R 21    // 板子右侧按键

#define MCP_PIN_L1 8      // 左拨杆1，在mcp2307上的引脚     左1---左2---右1---右2
#define MCP_PIN_L2 11     // 左拨杆2，在mcp2307上的引脚
#define MCP_PIN_R1 10     // 右拨杆1，在mcp2307上的引脚
#define MCP_PIN_R2 9      // 右拨杆2，在mcp2307上的引脚

#define MCP_PIN_UP 7	  // 功能键 上    
#define MCP_PIN_DOWN 4	  // 功能键 下    
#define MCP_PIN_LEFT 6	  // 功能键 左   
#define MCP_PIN_RIGHT 5  // 功能键 右   
#define MCP_PIN_O 3	  // 功能键 O     
#define MCP_PIN_X 2	  // 功能键 X     
#define MCP_PIN_A 1	  // 功能键 A    
#define MCP_PIN_B 0	  // 功能键 B     

#endif

