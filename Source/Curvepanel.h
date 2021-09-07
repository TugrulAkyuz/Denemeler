/*
  ==============================================================================

    Curvepanel.h
    Created: 6 Sep 2021 12:49:37pm
    Author:  Tuğrul Akyüz

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class CurvePanel : public juce::Component
{
    public :
    CurvePanel()
    {
        Control_1 = {0,0};
        addMouseListener(this, true);
        
    }
    
    void paint(juce::Graphics &g) override
    {
        g.fillAll(juce::Colours::darkgrey);

  

            g.setColour(juce::Colour(0xff4a4a4a));
        for (int x = 0; x < getWidth(); x += 8)
            g.drawLine(x, 0, x, getHeight());
        for (int y = 0; y < getHeight(); y += 8)
            g.drawLine(0, y, getWidth(), y);

        p.clear();
        
        p.startNewSubPath(0, 0);
       // p.cubicTo(Control_1, Control_2,juce::Point<float>(getWidth(),getHeight()));
       // p.quadraticTo(Control_1,juce::Point<float>(getWidth(),getHeight()));

        for (auto x = 0;   x < getWidth() ; x = x + 4)
        {
             p.lineTo(x, getHeight()/2 + (getHeight()/2)*sin(3.14*x*1.0/ getHeight()));
        }
        g.setColour (juce::Colours::yellow);
        g.strokePath(p,juce::PathStrokeType (3));
        g.setColour (juce::Colours::blue);
        g.drawEllipse (pointOnPath.getX(), pointOnPath.getY(), 4, 4, 3);
        g.setColour (juce::Colours::white);
        juce::Line<float> l(xx,0,xx,getHeight());
        g.drawLine(l, 2);
        p.intersectsLine(l);
       
        g.drawEllipse (pointOnPath2.getX(), pointOnPath2.getY(), 4, 4, 3);
        
     
       
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
    
    juce:: Point<float> Control_1;
    juce:: Point<float> Control_2;
    int selected_point;
    juce::Path p;
    juce::Point<float> pointOnPath;
    juce::Point<float> pointOnPath2;
    int xx;
    
private:
    
};
