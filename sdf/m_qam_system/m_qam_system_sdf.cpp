# include "m_qam_transmitter.h"
# include "sink.h"

int main(){

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################
	
	OpticalSignalXY S1("S1.sgn");
	Binary S2("S2.sgn");

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	MQamTransmitter B1{ vector<Signal*> {}, vector<Signal*> { &S1, &S2 } };
	B1.set(0);
	B1.setNumberOfBits(100);
	B1.setSeeBeginningOfImpulseResponse(false);

	Sink B2{ vector<Signal*> { &S1 }, vector<Signal*> {} };
	B2.setDisplayNumberOfSamples(true);

	Sink B3{ vector<Signal*> { &S2 }, vector<Signal*> {} };
	B2.setDisplayNumberOfSamples(true);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3 } };
	//System MainSystem{ vector<Block*> { &B1, &B2 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################
	
	MainSystem.run();

	return 0;

}
