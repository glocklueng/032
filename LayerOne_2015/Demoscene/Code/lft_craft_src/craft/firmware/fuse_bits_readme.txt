The ATmega88 should be configured for a 20 MHz crystal oscillator and no
internal clock divider. This corresponds to the following fuse bits:

	LOW:		11100111	(write e7, read e7)
	HIGH:		11011111	(write df, read df)
	EXTENDED:	xxxxx001	(write f9, read 01)

You need to program the LOW fuse byte with e7. Keep the factory defaults for
the others fuse bytes.
