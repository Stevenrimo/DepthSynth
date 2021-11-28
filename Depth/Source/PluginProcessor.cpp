/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DepthAudioProcessor::DepthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    

}

DepthAudioProcessor::~DepthAudioProcessor()
{
}

//==============================================================================
const juce::String DepthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DepthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DepthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DepthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DepthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DepthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DepthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DepthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DepthAudioProcessor::getProgramName (int index)
{
    return {};
}

void DepthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DepthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
            
        }
    }
    // prepares the filter to play 
    filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void DepthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DepthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DepthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            // checks to see if the voice can be cased into a SynthVoice
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {

                // TODO Osc controls
                // ADSR
                // uses the identifier of the value tree states to get the Raw (Atomic Float) value of the ADSR 
                auto& rAttack = *apvts.getRawParameterValue("ATTACK");
                auto& rDecay = *apvts.getRawParameterValue("DECAY");
                auto& rSustain = *apvts.getRawParameterValue("SUSTAIN");
                auto& rRelease = *apvts.getRawParameterValue("RELEASE");

                // gets the wave type of the oscillator 1 
                auto& rOsc1WaveType = *apvts.getRawParameterValue("OSC1WAVETYPE");
                auto& rFMWaveType = *apvts.getRawParameterValue("FMOSCWAVETYPE");

                // gets the paramters for FM
                auto& rDepth = *apvts.getRawParameterValue("FMDEPTH");
                auto& rFreq = *apvts.getRawParameterValue("FMFREQ");

                // updates the oscillator class to use the selected wave type
                voice->getOscillator().setWaveType(rOsc1WaveType);
                voice->getOscillator().setFMWaveType(rFMWaveType);
                // updates the FM parameters
                voice->getOscillator().setFMParams(rDepth, rFreq);
                // updates the adsr with the current values from the valueTree, useing load because the floats are atomic floats and this saves time 
                voice->getAdsr().updateADSR(rAttack.load(), rDecay.load(), rSustain.load(), rRelease.load());

                //TODO LFO
            }
        }
    }
    // Logs timestamp of each midimessage that be used for debugging purposes
    for (const juce::MidiMessageMetadata metadata : midiMessages)
    {
        if (metadata.numBytes == 3)
        {
            juce::Logger::writeToLog("TimeStamp: " + juce::String(metadata.getMessage().getTimeStamp()));
        }
    }

    //calls the render voices method, to iterate through the voices 
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Filters sound processing
    // loads all updated filter parameters to be passed into filter
    auto& rFilterType = *apvts.getRawParameterValue("FILTERTYPE");
    auto& rFilterCutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
    auto& rFilterRes = *apvts.getRawParameterValue("FILTERRES");
    // updates teh filter parameters
    filter.updateParameters(rFilterType, rFilterCutoff, rFilterRes);
    // applies the filter paramters to the audio inside the current buffer
    filter.process(buffer); 
}

//==============================================================================
bool DepthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DepthAudioProcessor::createEditor()
{
    return new DepthAudioProcessorEditor (*this);
}

//==============================================================================
void DepthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DepthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DepthAudioProcessor();
}

// By using the AudioProcessorValueTreeState class we are able to create parameters that can map to GUI elements - THIS IS THE INTERNAL AUDIO PROCCESING THAT IS DONE 
juce::AudioProcessorValueTreeState::ParameterLayout DepthAudioProcessor::createParams()
{
    // ComboBox: for giving users the ability to select oscillators
    // Attack -  float
    // Decay - float
    // Sustain - float
    // Release - float 
    // Filter type - combobox
    // Filter Cutoff - float
    // Filter Resonance - float 

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; 

    //Oscillator selector
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

    // FM 
    // FM frequency 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ", "FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    // FM Depth 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));

    // Oscillator 1 selector 
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine","Saw","Square" }, 0));

    // FM selector 
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FMOSCWAVETYPE", "FM Wave Type", juce::StringArray{ "Sine","Saw","Square" }, 0));

    // ADSR parameters
    // Attack
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.01f }, 0.1f));
    // Decay 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.01f}, 0.1f));
    // Sustain - Default of 1 so that as long as the user is holding down the note it will continue to play
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, 0.01f}, 1.0f));
    //Release - set release to be up to 3 seconds long 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 3.0f, 0.01f }, 0.4f));

    //Filter
    // Filter type selector 
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Lowpass","Bandpass","Highpass" }, 0));
    //Filter cutoff between 10hz and 20,000 hz 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> {10.0f, 20000.0f, 0.1f, 0.5f }, 16000.0f));
    //Filter Resonance
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> {1.0f, 10.0f, 0.1f}, 1.0f));

    return { params.begin(), params.end() };
}