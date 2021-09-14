/*
 ==============================================================================
 
 Curvepanel.h
 Created: 6 Sep 2021 12:49:37pm
 Author:  Tuğrul Akyüz
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

enum
{
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
    
};

class CurvePanel : public juce::Component ,
public juce::Timer
{
    public :
    CurvePanel()
    {
        Control_1 = {0,0};
        addMouseListener(this, true);
        attack_time = 0;
        decay_time = 1.0f;
        sustain_value = 1.0;
        release_time = 0;;
        startTimer(10);
        t = 0;
    }
    
    void timerCallback() override
    {
     
 
        repaint();
    }
    
    void paint(juce::Graphics &g) override
    {
        
        auto area = getLocalBounds();
        
        
        
        g.fillAll(juce::Colours::white);
        
        juce::DropShadow ds(juce::Colours::black.withAlpha(0.8f), 2, {2, 2});
        ds.radius = 10;
        ds.offset = { 0, 0 };
        juce::Rectangle<int> r(area);

       
          ds.drawForRectangle(g, r);
        g.setColour(juce::Colours::black);
        g.drawRect(r,2);
        
        //g.setColour(juce::Colour(0xff4a4a4a));
        g.setColour(juce::Colour(0x998a8a8a));
        for (int x = 0; x < getWidth(); x += 16*1.618)
            g.drawLine(x, 0, x, getHeight());
        for (int y = 0; y < getHeight(); y += 16)
            g.drawLine(0, y, getWidth(), y);
        
        p.clear();
        juce::PathStrokeType stroke(3, juce::PathStrokeType::JointStyle::curved,
                                    juce::PathStrokeType::EndCapStyle::rounded);
             
        p.startNewSubPath(0, getHeight());
        // p.cubicTo(Control_1, Control_2,juce::Point<float>(getWidth(),getHeight()));
        // p.quadraticTo(Control_1,juce::Point<float>(getWidth(),getHeight()));
        
//        for (auto x = 0;   x < getWidth() ; x = x + 4)
//        {
//            p.lineTo(x, getHeight()/2 + aa*(getHeight()/2)*sin(3.14*x*1.0/ getHeight()));
//        }
        
        // attack
        int attack_pos = attack_time*getWidth()/4;
        int decay_pos = attack_pos + decay_time*getWidth()/4;
        int release_pos =  decay_pos + release_time* getWidth()/4 + getWidth()/4;
        int sustain_pos = (1-sustain_value) * 4*getHeight()/5 + getHeight()/5;
        
        g.setColour(juce::Colours::yellow);
        g.drawVerticalLine(attack_pos, getHeight()/5, getHeight());
        g.drawVerticalLine(decay_pos , sustain_pos , getHeight());
        g.drawVerticalLine(decay_pos + getWidth()/4 , sustain_pos ,getHeight());
        
        //p.lineTo(attack_pos,1*getHeight()/5);
        p.quadraticTo(attack_pos/2, getHeight(), attack_pos, 1*getHeight()/5) ;
        
        //p.lineTo(decay_pos,sustain_pos );
        p.quadraticTo((decay_pos + attack_pos)/2, sustain_pos, decay_pos, sustain_pos) ;
        p.lineTo(decay_pos + getWidth()/4, sustain_pos);
        //p.lineTo(release_pos,getHeight() );
        p.quadraticTo((release_pos + decay_pos + getWidth()/4)/2, getHeight(), release_pos, getHeight()) ;
        //p.closeSubPath();
         
        juce::ColourGradient cg(juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.8f), getWidth()/2 , 0,
                               juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f), getWidth()/2,  getHeight() , false);
        
        g.setGradientFill(cg);;
        g.fillPath(p);
        
        
        
//        juce::DropShadow ds(juce::Colours::white.withAlpha(0.85f), 2, {2, 2});
//        ds.radius = 30;
//        ds.offset = { 0, 10 };
//        ds.drawForPath(g, p);
        g.setColour (juce::Colours::orange);
        juce::Rectangle<int> rp (0,0,6,6);
        rp.translate(attack_pos-3,1*getHeight()/5 -3);
        g.drawRect(rp);
        rp =  {0,0,6,6};
        rp.translate(decay_pos -3, sustain_pos -3);
        g.drawRect(rp);
        rp =  {0,0,6,6};
        rp.translate( release_pos -3, getHeight() -3 );
        g.drawRect(rp);
        
        
        
        g.setColour (juce::Colours::yellow);
        g.strokePath(p,stroke);
        g.setColour (juce::Colours::blue);
        
        g.setColour (juce::Colours::white);
        //juce::Line<float> l(xx,0,xx,getHeight());
       // g.drawLine(l, 2);
        

 /*       t++;
 
        int m;

      
        if(( t*getHeight() / (attack_pos + 0.01)) )
           m = 

        juce::Point<float> pt = p.getPointAlongPath(m);
        g.drawEllipse(pt.getX()-5,pt.getY()-5, 10, 10, 1);*/
        
        
    }
    void resized() override
    {
        
        
    }
    void mouseDown(const juce::MouseEvent& ev) override
    {
        //        if(Control_1.getDistanceFrom(ev.position) < Control_2.getDistanceFrom(ev.position))
        //            selected_point = 0;
        //        else
        //            selected_point = 1;
        
    }
    void mouseDrag(const juce::MouseEvent& ev) override
    {
        //       if(selected_point == 0)
        Control_1 = ev.position;
        //       else
        //           Control_2 = ev.position;
        
        repaint();
        
    }
    
    void mouseMove(const juce::MouseEvent& ev) override
    {
        
        
        p.getNearestPoint(ev.position,pointOnPath);
        
        pointOnPath2 = p.getPointAlongPath(ev.position.getX());
        
        xx = ev.position.getX();
        
        DBG("X:" << pointOnPath.getX()<<" Y:" << pointOnPath.getY());
        
        repaint();
        
    }
    
    void setAmp(float a)
    {
        aa = a;
        repaint();
    }
    void setADSF(int type, float value)
    {
        switch(type)
        {
    case  ATTACK  :
                attack_time = value;
        break;
    case  DECAY  :
                decay_time = value;
        break;
    case  SUSTAIN  :
                sustain_value =  value;
        break;
    case  RELEASE  :
                release_time =  value;
        break;
        }
        
        repaint();
    }
    
    
    int t;
    float aa;
    juce:: Point<float> Control_1;
    juce:: Point<float> Control_2;
    int selected_point;
    juce::Path p;
    juce::Point<float> pointOnPath;
    juce::Point<float> pointOnPath2;
    int xx;
    
    float attack_time;
    float decay_time;
    float sustain_value;
    float release_time;
    
private:
    
};
