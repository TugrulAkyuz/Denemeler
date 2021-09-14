#pragma once

#include <JuceHeader.h>
#include "Curvepanel.h"
#include "AyriRect.h"
 


class MyLookAndFeel : public juce::LookAndFeel_V4
{
private:
    void drawRotarySlider(juce::Graphics & g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider & slider) override
    {
    
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour (juce::Slider::rotarySliderFillColourId);
        
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (20);
        
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;
        

        
        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);
        
        g.setColour (outline);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);
            
            g.setColour (fill);
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        
        auto thumbWidth = lineW * 2.0f;
        juce::Point<float> thumbPoint (bounds.getCentreX() + (arcRadius - 5) * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                                       bounds.getCentreY() + (arcRadius - 5) * std::sin (toAngle - juce::MathConstants<float>::halfPi));
        
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        // g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
        g.drawLine(backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(), thumbPoint.getX(), thumbPoint.getY(),lineW);
        
        
    }
    
};
class CustomRoratySlider :public juce::Slider{
public:
    CustomRoratySlider()
    {
        
        setLookAndFeel(&myLookAndFeel);
        //        auto rotaryparams = getRotaryParameters();
        //        rotaryparams.startAngleRadians = juce::MathConstants<float>::pi *1.5;
        //        rotaryparams.endAngleRadians = juce::MathConstants<float>::pi *3.0;
        //        setRotaryParameters(rotaryparams);
        setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        //
        setRange(0.0f, 100.0f,1);
        setTextBoxStyle(juce::Slider::TextBoxAbove, true, 120, 20);
        
        //
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::orange);
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
        setColour(Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        
        
        
    }
    ~CustomRoratySlider(){
        setLookAndFeel(nullptr);
        
    }
private:
    //juce:
    
    void paint(juce::Graphics & g) override
    {
        auto sBounds = getLocalBounds();
        getLookAndFeel().drawRotarySlider(g,
                                          sBounds.getX(),
                                          sBounds.getY(),
                                          sBounds.getWidth(),
                                          sBounds.getHeight(),
                                          juce::jmap(getValue(),getRange().getStart(),getRange().getEnd(),0.0,1.0),
                                          juce::MathConstants<float>::pi *1.5,
                                          juce::MathConstants<float>::pi *3.0,
                                          *this);
        
       g.drawFittedText(juce::String(getRange().getStart()), sBounds, juce::Justification::centredLeft, 1);
       g.drawFittedText(juce::String(getRange().getEnd()), sBounds, juce::Justification::centredBottom, 1);
        
        
        
    }
    MyLookAndFeel myLookAndFeel;
    
    
};

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent  : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
                        
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    //==============================================================================
    // Your private member variables go here...
    
    CustomRoratySlider    slider1;
    CustomRoratySlider    slider2;
    CustomRoratySlider    slider3;
    CustomRoratySlider    slider4;
    
    CustomRoratySlider    slider11;
    CustomRoratySlider    slider21;
    CustomRoratySlider    slider31;
    CustomRoratySlider    slider41;
    
    juce::ToggleButton button1;
    CurvePanel myCurvePanel;
 
    juce::ADSR myADSR;
    juce::ADSR::Parameters myADSR_Params;
    
    MyLookAndFeel myLookAndFeel;
    
    AyriRect myAyriRect;
    
    
    
    std::vector <juce::Component *> getAllSliders(){
        
        return {
            &slider1,
            &slider2,
            &slider3,
            &slider4,
            &slider11,
            &slider21,
            &slider31,
            &slider41

        };
 
   
        
    }
    void buttonClicked (juce::Button* button) override // [2]
    {
        if (button == &button1)                                                      // [3]
           {
               
               resized();
           }
        
    }
    
     void sliderValueChanged (juce::Slider* slider) override
     {
         if(slider == &slider1)
         {
             myCurvePanel.setADSF(ATTACK,slider->getValue()*1.0/100);
         }
         if(slider == &slider2)
         {
             myCurvePanel.setADSF(DECAY,slider->getValue()*1.0/100);
         }
         if(slider == &slider3)
         {
             myCurvePanel.setADSF(SUSTAIN,slider->getValue()*1.0/100);
         }
         if(slider == &slider4)
         {
             myCurvePanel.setADSF(RELEASE,slider->getValue()*1.0/100);
         }
     }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
