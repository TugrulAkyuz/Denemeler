#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    setSize (400, 600);
    juce::LookAndFeel::setDefaultLookAndFeel(&myLookAndFeel);
    addAndMakeVisible (button1);
    button1.setButtonText ("BASSSSS");
    button1.addListener(this);
    for(auto * comp : getAllSliders())
    {
        addAndMakeVisible(comp);
        ((CustomRoratySlider *)(comp))->addListener(this);
        ((CustomRoratySlider *)(comp))->setValue(50);
        
    }
 
    button1.setState(juce::Button::ButtonState::buttonDown);
 
    slider11.setVisible(false);
    slider21.setVisible(false);
    slider31.setVisible(false);
    slider41.setVisible(false);
    
    addAndMakeVisible(myCurvePanel);
    addAndMakeVisible(myAyriRect);
    //slider1.addListener(this);
    
    
    
    myADSR.setSampleRate(44000);
     
    
}

MainComponent::~MainComponent()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    button1.removeListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::grey);
    
    g.setFont (juce::Font (16.0f));
    
    juce::ColourGradient cg(juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.3f), getWidth()/2 , 0,
                            juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.3f), 0,  getHeight() , true);
    g.setGradientFill(cg);
    g.fillAll();
    //g.setColour (cg);
    //  g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto bounds = getLocalBounds();
    bounds.reduce(50, 50);
    
    auto aaa = bounds.removeFromTop(240);
    myCurvePanel.setBounds(aaa.removeFromLeft(aaa.getWidth()/2));
    myAyriRect.setBounds(aaa);
    
    
    auto buttonarea = bounds.removeFromTop(50);
    
    
    auto top = bounds.removeFromTop(bounds.getHeight()/2);
    auto buttom = bounds;
    auto a = top.removeFromLeft(bounds.getWidth()/2);
    auto b = buttom.removeFromLeft(bounds.getWidth()/2);
    //auto tmpBound = bounds.removeFromTop();
    button1.setBounds(buttonarea);

        slider1.setBounds(a);
        slider2.setBounds(b);
        slider3.setBounds(top);
        slider4.setBounds(buttom);


        slider11.setBounds(a);
        slider21.setBounds(b);
        slider31.setBounds(top);
        slider41.setBounds(buttom);

        
    if(button1.getToggleState()==0)
    {
        
        slider1.setVisible(true);
        slider2.setVisible(true);
        slider3.setVisible(true);
        slider4.setVisible(true);
        
        slider11.setVisible(false);
        slider21.setVisible(false);
        slider31.setVisible(false);
        slider41.setVisible(false);

    }
    else
    {
        slider11.setVisible(true);
        slider21.setVisible(true);
        slider31.setVisible(true);
        slider41.setVisible(true);
        
        slider1.setVisible(false);
        slider2.setVisible(false);
        slider3.setVisible(false);
        slider4.setVisible(false);

    }
    

    

    //tmpBound = bounds.removeFromTop(getHeight()/2).removeFromRight(getWidth()/2);
    //slider2.setBounds(tmpBound);
    
}
