/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    g.setColour(juce::Colours::white);

    if (mShouldBePainting)
    {
        juce::Path p;
        mAudioPoints.clear();
        auto waveform = audioProcessor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);

        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }

        p.startNewSubPath(0, getHeight() / 2);

        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float> (mAudioPoints[sample], -1.0f, 1.0f, 200, 0);

            p.lineTo(sample, point);
        }

        g.strokePath(p, juce::PathStrokeType(2));

        mShouldBePainting = false;

    }


    /*.setColour(juce::Colours::white);
    g.setFont(15.0f);

    if (audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::green);
        
        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 20, 200, 20, juce::Justification::centred);
    }
    else
    {
        g.drawText("Load Sound", getWidth() / 2 - 50, getHeight() / 2 - 20, 200, 20, juce::Justification::centred);
    }*/
}

void SamplerAudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool SamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    return false;
}

void SamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            mShouldBePainting = true;
            audioProcessor.loadFile(file);
        }
    }

    repaint();
}