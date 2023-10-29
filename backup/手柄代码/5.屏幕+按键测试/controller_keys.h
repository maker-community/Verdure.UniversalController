// controller_keys.h

#ifndef _CONTROLLER_KEYS_h              // ��ֹ�ظ�����
#define _CONTROLLER_KEYS_h

#if defined(ARDUINO) && ARDUINO >= 100  // arduino�İ汾>= 100��
	#include "arduino.h"                // ���ͷ�ļ������� Arduino �������Ļ�������ͺ���������
#else
	#include "WProgram.h"               // �ھɰ汾�� Arduino �����У�"WProgram.h" ����Ҫ��ͷ�ļ������ڶ�����������ͱ�����
#endif

/*  -----------�˴�Ϊ�°��ӵ�����-------------------
#define PIN_LX 2          // ��ҡ��X
#define PIN_LY 1		  // ��ҡ��Y
#define PIN_L_KNOB 3	  // ����ť
#define PIN_L_UP 46		  // ����ϰ���
#define PIN_L_DOWN 45     // ����°���
				  
#define PIN_RX 16         // ��ҡ��X
#define PIN_RY 15		  // ��ҡ��Y
#define PIN_R_KNOB 14	  // ����ť
#define PIN_R_UP 44		  // �Ҳ��ϰ���	  
#define PIN_R_DOWN 39     // �Ҳ��°���
						  
#define MCP_PIN_L1 7      // ��ದ��1 ��mcp2307���ӵ�����     ��1---��2---��1---��2
#define MCP_PIN_L2 4      // ��ದ��2 ��mcp2307���ӵ�����
#define MCP_PIN_R1 5      // �Ҳದ��1 ��mcp2307���ӵ�����
#define MCP_PIN_R1 6      // �Ҳದ��2 ��mcp2307���ӵ�����
						  
#define MCP_PIN_UP 8	  // ���ܼ� ��
#define MCP_PIN_DOWN 11	  // ���ܼ� ��
#define MCP_PIN_LEFT 9	  // ���ܼ� ��
#define MCP_PIN_RIGHT 10  // ���ܼ� ��
#define MCP_PIN_O 12	  // ���ܼ� O
#define MCP_PIN_X 13	  // ���ܼ� X
#define MCP_PIN_A 14	  // ���ܼ� A
#define MCP_PIN_B 15	  // ���ܼ� B
*/


// --------------------- ����Ϊ����İ��ӵ���ʱ���ã��°��ӵ��ֺ�ɾ��----------------------------------
#define PIN_LX 2          // ��ҡ��X
#define PIN_LY 1		  // ��ҡ��Y
#define PIN_L_KNOB 3	  // ����ť
#define PIN_L_UP 46		  // ����ϰ���
#define PIN_L_DOWN 39    // ����°���

#define PIN_RX 16         // ��ҡ��X
#define PIN_RY 15		  // ��ҡ��Y
#define PIN_R_KNOB 14	  // ����ť
#define PIN_R_UP 45		  // �Ҳ��ϰ���
#define PIN_R_DOWN 44     // �Ҳ��°���

#define PIN_BOARD_L 0     // ������ఴ��
#define PIN_BOARD_R 21    // �����Ҳఴ��

#define MCP_PIN_L1 8      // �󲦸�1����mcp2307�ϵ�����     ��1---��2---��1---��2
#define MCP_PIN_L2 11     // �󲦸�2����mcp2307�ϵ�����
#define MCP_PIN_R1 10     // �Ҳ���1����mcp2307�ϵ�����
#define MCP_PIN_R2 9      // �Ҳ���2����mcp2307�ϵ�����

#define MCP_PIN_UP 7	  // ���ܼ� ��    
#define MCP_PIN_DOWN 4	  // ���ܼ� ��    
#define MCP_PIN_LEFT 6	  // ���ܼ� ��   
#define MCP_PIN_RIGHT 5  // ���ܼ� ��   
#define MCP_PIN_O 3	  // ���ܼ� O     
#define MCP_PIN_X 2	  // ���ܼ� X     
#define MCP_PIN_A 1	  // ���ܼ� A    
#define MCP_PIN_B 0	  // ���ܼ� B     

#endif

