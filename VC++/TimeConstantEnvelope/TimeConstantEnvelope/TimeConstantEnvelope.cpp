// TimeConstantEnvelope.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

class EnvelopeAR {
public:
	EnvelopeAR(int _attack, int _release, float _v0, float _v1, float _v2, float _attackTauRatio=0.36f, float _releaseTauRatio=0.36f) :
		amplitude(_v0),
		v0(_v0),
		v1(_v1),
		v2(_v2),
		vLast(_v0),
		attackTauRatio(_attackTauRatio),
		releaseTauRatio(_releaseTauRatio)
	{
		setAttack(_attack);
		setRelease(_release);
	}

	~EnvelopeAR() {}

	void setAttack(int _attack) {
		attack = _attack;
		tau0 = attack * attackTauRatio;
	}
	int getAttack() { return attack; }

	void setRelease(int _release) {
		release = _release;
		tau1 = release * releaseTauRatio;
	}
	int getRelease() { return release; }

	void setAttackTauRatio(float _attackTauRatio) { attackTauRatio = _attackTauRatio; }
	float getAttackTauRatio() { return attackTauRatio;  }

	void setReleaseTauRatio(float _releaseTauRatio) { releaseTauRatio = _releaseTauRatio; }
	float getReleaseTauRatio() { return releaseTauRatio; }

	void setV0(float _v0) { v0 = _v0; }
	float getV0() { return v0; }
	void setV1(float _v1) { v1 = _v1; }
	float getV1() { return v1; }
	void setV2(float _v2) { v2 = _v2; }
	float getV2() { return v2; }

	float getAmplitude() { return amplitude; }
	float getAmplitude(int tick) {
		if (tick < attack) {
			// attack‚Ìˆ—
			amplitude = v0 + (v1 - v0) * (1 - expf(-(float)tick / tau0));
			vLast = amplitude;
		}
		else {
			// release‚Ìˆ—
			amplitude = (vLast - v2) * (expf(-(float)(tick - attack) / tau1)) + v2;
		}
		return amplitude;
	}

private:
	int attack;
	int release;
	float amplitude;
	float v0;
	float v1;
	float v2;
	float vLast;
	float tau0;
	float tau1;
	float attackTauRatio;
	float releaseTauRatio;
};

#define PI_F			(3.1415926f)
#define PERIOD			(16000)
#define ENVELOPE_CYCLE	(16)

#define ENVELOPE_PERIOD		(500)
#define FREQUENCY_ATTACK	(5)
#define FREQUENCY_RELEASE	(300)
#define AMPLITUDE_ATTACK	(50)
#define AMPLITUDE_RELEASE	(200)

EnvelopeAR envelopeFrequency(FREQUENCY_ATTACK, FREQUENCY_RELEASE, 880.0f, 120.0f, 40.0f, 0.36f, 0.1f);
EnvelopeAR envelopeAmplitude(AMPLITUDE_ATTACK, AMPLITUDE_RELEASE, 0.9f, 1.0f, 0.0f);

int ticks = 0;
int envelopeTicks = 0;
float frequency;
float phi;
float phiDelta;
float amplitude;

void generateWave()
{
	phi += phiDelta;
	if (phi >= 1.0f) {
		phi -= 2.0f;
	}
	float level = sinf(PI_F * phi) * amplitude;
	//printf("%d\t%f\t%f\t%f\n", ticks, frequency, amplitude, level);
	printf("%f\n", level);
}

void generateEnvelope()
{
	// Frequency Envelope
	frequency = envelopeFrequency.getAmplitude(envelopeTicks);
	phiDelta = 2.0f * frequency / PERIOD;

	// Amplitude Envelope
	amplitude = envelopeAmplitude.getAmplitude(envelopeTicks);

	envelopeTicks++;
	if (envelopeTicks == ENVELOPE_PERIOD) {
		envelopeTicks = 0;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	for (; ticks < PERIOD; ticks++) {
		if (ticks % ENVELOPE_CYCLE == 0) {
			generateEnvelope();
		}
		generateWave();
	}

	return 0;
}


