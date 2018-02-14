
#include "netxpto_20180118.h"
#include <stdio.h>

#include "binary_source_20180118.h"
#include "clock_20171219.h"
#include "alice_qkd.h"
#include "single_photon_source_20171218.h"
#include "polarizer_20180113.h"
#include "sink_20180118.h"
#include "bob_qkd.h"
#include "polarization_beam_splitter_20180109.h"
#include "single_photon_detector_20180206.h"
#include "super_block_interface_20180118.h"
#include "detection_decision_circuit_20180206.h"

int main()
{
	// #####################################################################################################
	// ################################# System Input Parameters #########################################
	// #####################################################################################################

	double RateOfPhotons{ 1e3 };
	vector<t_iqValues> iqAmplitudeValues = { {0.0,0.0},{-45.0,0.0},{90.0,0.0},{45.0,0.0} };
	int NumberOfSamplesPerSylbom{ 16 };
	double DetectorWindowTimeOpen{0.07};		// in units of time (ms), symbol period is equals to 1 ms, so this value must be smaller than 1 ms
	double DetectorPulseDelay{ 0.0};		// in units of time (ms)
	double DetectorProbabilityDarkCount{ 0.01 };

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	// Alice Implementation
	Binary NUM_A{ "NUM_A.sgn" };
	NUM_A.setSaveSignal(false);

	Binary Bin_1{ "Bin_1.sgn" };
	Bin_1.setSaveSignal(false);

	Binary Bin_2{ "Bin_2.sgn" };
	Bin_2.setSaveSignal(false);

	Binary Bin_1out{ "Bin1_out.sgn" };
	Bin_1out.setSaveSignal(false);

	Binary MI_A{ "MI_A.sgn" };
	MI_A.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_A{"CLK_A.sgn"};
	CLK_A.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_A_1{ "CLKA_out.sgn" };
	CLK_A_1.setSaveSignal(false);

	PhotonStreamXY S2 {"S2.sgn"};
	S2.setSaveSignal(false);

	TimeContinuousAmplitudeDiscreteReal S3{ "S3.sgn" };
	S3.setSaveSignal(false);

	Messages C_C_1{ "C_C_1.sgn" };
	C_C_1.setSaveSignal(false);

	Messages C_C_6{ "C_C_1.sgn" };
	C_C_6.setSaveSignal(false);

	PhotonStreamXY Q_C_1{"Q_C_1.sgn"};

	// Bob Implementation
	Binary NUM_B{ "NUM_B.sgn" };
	NUM_B.setSaveSignal(false);

	Binary Bin_6{ "Bin_6.sgn" };
	Bin_6.setSaveSignal(false);

	Binary MI_B{ "MI_B.sgn" };
	MI_B.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_B{"CLK_B.sgn"};
	CLK_B.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_B_out1{ "CLKB_out1.sgn" };

	TimeContinuousAmplitudeContinuous CLK_B_out2{ "CLKB_out2.sgn" };

	TimeContinuousAmplitudeDiscreteReal S4{ "S4.sgn" };
	S4.setSaveSignal(false);

	PhotonStreamXY S5{ "S5.sgn" };
	//S5.setSaveSignal(false);

	PhotonStreamMPXY S6{ "S6.sgn" };
	
	TimeContinuousAmplitudeContinuous S7{ "S7.sgn" };
	TimeContinuousAmplitudeContinuous S9{ "S9.sgn" };

	PhotonStreamXY Q_C_4{"Q_C_4.sgn"};

	Messages C_C_4{ "C_C_4.sgn" };
	C_C_4.setSaveSignal(false);

	Messages C_C_3{ "C_C_3.sgn" };
	C_C_3.setSaveSignal(false);



	
	
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ##################################
	// #####################################################################################################

	// Alice implementation
	BinarySource B0{ vector <Signal*> {}, vector <Signal*> {&NUM_A, &Bin_1, &Bin_2} };
	B0.setBitPeriod(1/(2 * RateOfPhotons));
	B0.setMode( DeterministicCyclic );
	B0.setBitStream("11");

	Clock B1{ vector<Signal*>{}, vector<Signal*>{&CLK_A} };
	B1.setClockPeriod(1 / RateOfPhotons);
	B1.setSamplingPeriod((1 / RateOfPhotons) / NumberOfSamplesPerSylbom);
	B1.setClockPhase(3*PI/2);
	

	AliceQKD B2{ vector <Signal*> { &CLK_A, &NUM_A, &Bin_1, &Bin_2}, vector <Signal*> {&CLK_A_1, &S3, &Bin_1out, &MI_A} };
	B2.setIqAmplitudes(iqAmplitudeValues);
	B2.setNumberOfSamplesPerSymbol(NumberOfSamplesPerSylbom);
	B2.setPulseShaperFilter(Square);
	B2.setSaveInternalSignals(false);
	

	SinglePhotonSource B3{ vector <Signal*> {&CLK_A_1}, vector <Signal*> {&S2} };
	
	Polarizer B4{ vector<Signal*>{&S2,&S3}, vector <Signal*> {&Q_C_1} };

	
	Sink BB2{ vector <Signal*> {&Bin_1out}, vector <Signal*> {} };
	BB2.setNumberOfSamples(1000);
	BB2.setDisplayNumberOfSamples(true);

	Sink BB4{ vector <Signal*> {&MI_A}, vector <Signal*> {} };
	BB4.setNumberOfSamples(1000);
	BB4.setDisplayNumberOfSamples(true);

	// ################################################################################


	//Bob Implementation
	BinarySource B5{ vector <Signal*> {}, vector <Signal*> {&NUM_B, &Bin_6} };
	B5.setBitPeriod(1 / (RateOfPhotons));
	B5.setMode(DeterministicCyclic);
	B5.setBitStream("1");

	Clock B6{ vector <Signal*> {}, vector <Signal*> {&CLK_B} };
	B6.setClockPeriod(1 / RateOfPhotons);
	B6.setSamplingPeriod((1 / RateOfPhotons) / NumberOfSamplesPerSylbom);
	B6.setClockPhase(3 * PI / 2);
	

	BobQKD B7{ vector<Signal*>{&NUM_B, &CLK_B, &S7, &S9, &Bin_6}, vector<Signal*>{&S4, &CLK_B_out1, &CLK_B_out2} };
	B7.setPulseShaperFilter(Square);
	B7.setNumberOfSamplesPerSymbol(NumberOfSamplesPerSylbom);
	B7.setPulseShaperFilter(Square);
	B7.setPulseWidth(DetectorWindowTimeOpen);
	B7.setPulseDelay(DetectorPulseDelay);
	//B7.setSaveInternalSignals(false);
	
	
	Polarizer B10(vector<Signal*>{&Q_C_1, &S4}, vector<Signal*>{&S5});

	PolarizationBeamSplitter B8(vector<Signal*>{&S5}, vector<Signal*>{&S6});

	SinglePhotonDetector B11(vector<Signal*>{&S6, &CLK_B_out1}, vector<Signal*>{&S7});
	B11.setPath(0);
	B11.setProbabilityDarkCount(DetectorProbabilityDarkCount);
	
	SinglePhotonDetector B9(vector<Signal*>{&S6, &CLK_B_out2}, vector<Signal*>{&S9});
	B9.setPath(1);
	B9.setProbabilityDarkCount(DetectorProbabilityDarkCount);


	
	

	
	

	// ####################################################################################################
	// ########################### System Declaration and Inicialization ##################################
	// #####################################################################################################

	System MainSystem{ vector <Block*> {&B0, &B1, &B2, &B3, &B4, &BB4, &BB2, &B5, &B6, &B7, &B10, &B8, &B11,&B9} };
	// ####################################################################################################
	// ########################### Run ##################################
	// #####################################################################################################

	MainSystem.run();


    return 0;
}

