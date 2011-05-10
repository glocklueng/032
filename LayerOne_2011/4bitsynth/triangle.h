//Maximum and minimum timer values for aesthetic purposes
#define MAXIMUM_FREQ 19000
#define MINIMUM_FREQ 60

//USART Settings for 20MHz operation
#define USART_BAUDRATE 31250
#define BAUD_PRESCALE 39

//MIDI Message types
#define MIDI_STATUS_TYPE_NOTEON 0x90
#define MIDI_STATUS_TYPE_NOTEOFF 0x80
#define MIDI_STATUS_TYPE_CC 0xB0
#define MIDI_STATUS_TYPE_PB 0xE0

//Local MIDI status identifying constants
#define	MIDI_STATUS_NONE 0
#define	MIDI_STATUS_NOTEON 1
#define	MIDI_STATUS_NOTEOFF 2
#define	MIDI_STATUS_CC 3
#define	MIDI_STATUS_PB 4

unsigned char midi_channel;
unsigned char byte_received;
unsigned char playing_midi_note;
unsigned char current_midi_status;
unsigned char current_midi_channel;
unsigned char current_midi_note;
unsigned int fine_pitch_bend;
unsigned char current_midi_cc;
unsigned char current_midi_ccdata;
unsigned char current_midi_pb_l;
unsigned char current_midi_pb_h;
unsigned int current_midi_pb;

unsigned int fake_16_timer; //to slow down decay/pitch timer further

unsigned char fivebit_counter;
unsigned char note_on_gate;

#define SWEEP_UP 1
#define SWEEP_DOWN 0

enum cc_mesages {
	DUTY_CYCLE_CC = 1,
	MASTER_VOLUME_CC = 7,
	FINE_PITCH_CC = 8,
	VOLDECAY_ENABLED_CC,
	VOLDECAY_AMOUNT_CC,
	VOLDECAY_LOOP_ENABLED_CC,
	SWEEP_ENABLED_CC,
	SWEEP_DIRECTION_CC,
	SWEEP_AMOUNT_CC,
	SWEEP_LOOP_ENABLED_CC
};

unsigned char num_bytes;
unsigned char num_ccs;
unsigned char num_pbs;

unsigned int frequency;	//timer1 countdown value
unsigned char amplitude;

unsigned char master_volume;

unsigned char sweep_enabled;
unsigned char sweep_direction;
unsigned char sweep_amount;
unsigned char sweep_loop_enabled;

//Pitch bend distance from current note's timer value
unsigned int distance;

unsigned char byte_ready;

void init_interrupts(void);
void enable_USART_interrupts(void);
void disable_USART_interrupts(void);
void init_io(void);
void note_on(void);
void note_off(void);
void process_cc(void);
void bend_pitch(void);
//void update_duty_cycle(void);
void update_frequency(unsigned int new_frequency);

void clear_byte_received(void);
void check_byte_received(void);
void check_channel_set(void); //A 1 on PD3 will use Channels 0-4, 0 will make it use Channels 5-9

void note_on(void);
void note_off(void);
void init_interrupts(void);
void init_io(void);
void init_timers(void);

//Timer values for musical notes
//For Prescaler = 1024

const unsigned int note_table[128] = { 38223, 36078, 34053, 32142, 30338,
		28635, 27028, 25511, 24079, 22728, 21452, 20248, 19112, 18039, 17027,
		16071, 15169, 14318, 13514, 12756, 12040, 11364, 10726, 10124, 9556,
		9020, 8514, 8036, 7585, 7159, 6757, 6378, 6020, 5682, 5363, 5062, 4778,
		4510, 4257, 4018, 3793, 3580, 3379, 3189, 3010, 2841, 2682, 2531, 2389,
		2255, 2129, 2009, 1897, 1790, 1690, 1595, 1505, 1421, 1341, 1266, 1195,
		1128, 1065, 1005, 949, 895, 845, 798, 753, 711, 671, 633, 598, 564,
		533, 503, 475, 448, 423, 399, 377, 356, 336, 317, 299, 282, 267, 252,
		238, 224, 212, 200, 189, 178, 168, 159, 150, 141, 134, 126, 119, 112,
		106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 48, 45, 42,
		40, 38, 36, 34, 32, 30, 28, 27, 25 };
