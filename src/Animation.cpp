#include "Animation.h"

Animation::Animation() {
	time_start = 0.0;
	time_end = 0.0;
	num_channels = 0;
}

Animation::~Animation() {

}

/* Parse .anim file */
bool Animation::load(Tokenizer& tokenizer) {
	
	//Seconds of animation, not necessarily corresponding to first and last keyframes
	tokenizer.FindToken("range");
	Animation::time_start = tokenizer.GetFloat();
	Animation::time_end = tokenizer.GetFloat();
	
	//Number of channels = 3 * number of joints in character + 3 for root translation
	tokenizer.FindToken("numchannels");
	Animation::num_channels = tokenizer.GetInt();
	
	//Every Channel has an extrap in and out, and a list of keyframes
	Channel* newChannel;
	Key* newKey;
	int num_keys;
	char text[256];
	tokenizer.FindToken("channel");
	
	//Animation has some number of Channels corresponding to DOFs
	for (int i = 0; i < Animation::num_channels; i++) {
		newChannel = new Channel();
		tokenizer.FindToken("extrapolate");
		tokenizer.GetToken(text);
		newChannel->extrap_in = Channel::Extrapolation::constant;
		tokenizer.GetToken(text);
		newChannel->extrap_out = Channel::Extrapolation::constant;

		//Every Key has time and value floats, plus tangent in and out denoted { flat, linear, smooth } or as a float slope
		tokenizer.FindToken("keys");
		num_keys = tokenizer.GetInt();
		tokenizer.FindToken("{");

		Animation::channels.push_back(newChannel);
		
		//Channel stores a list of Keyframes
		for (int j = 0; j < num_keys; j++) {
			newKey = new Key();

			newKey->time = tokenizer.GetFloat();
			newKey->value = tokenizer.GetFloat();

			tokenizer.GetToken(text);
			newKey->tangent_in = Key::Tangent::flat;
			tokenizer.GetToken(text);
			newKey->tangent_out = Key::Tangent::flat;

			if (j != 0) {
				newKey->prev = newChannel->keys[j - 1];
				newChannel->keys[j - 1]->next = newKey;
			}

			newChannel->keys.push_back(newKey);
		}
	}
	
	tokenizer.Close();
	
	return true;
}

/* Based on time elapsed, update Joint poses corresponding to values in DOF Channel */
void Animation::update(float time) {
	//Evaluate the current DOF value from each Channel
	
}