#include "Renderer.h"
#include <cmath>
#include <algorithm>

static SDL_Color bg{18,21,27,255};
static SDL_Color panel{29,34,43,255};
static SDL_Color panel2{35,41,52,255};
static SDL_Color border{67,77,94,255};
static SDL_Color white{235,239,245,255};
static SDL_Color muted{160,170,185,255};
static SDL_Color accent{76,175,120,255};
static SDL_Color blue{72,145,220,255};
static SDL_Color yellow{235,190,75,255};
static SDL_Color red{220,90,90,255};
static SDL_Color cyan{72,190,205,255};

Renderer::Renderer(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* smallFont)
    : r_(renderer), font_(font), smallFont_(smallFont) {}

void Renderer::rect(int x,int y,int w,int h, SDL_Color c, bool fill) {
    SDL_SetRenderDrawColor(r_,c.r,c.g,c.b,c.a);
    SDL_Rect q{x,y,w,h};
    if(fill) SDL_RenderFillRect(r_,&q); else SDL_RenderDrawRect(r_,&q);
}
void Renderer::text(const std::string& s,int x,int y,SDL_Color c,TTF_Font* f) {
    if(s.empty()) return;
    if(!f) f = font_;
    SDL_Surface* surf=TTF_RenderUTF8_Blended(f,s.c_str(),c);
    if(!surf) return;
    SDL_Texture* tex=SDL_CreateTextureFromSurface(r_,surf);
    SDL_Rect dst{x,y,surf->w,surf->h};
    SDL_RenderCopy(r_,tex,nullptr,&dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}
void Renderer::centered(const std::string& s,int x,int y,int w,SDL_Color c,TTF_Font* f) {
    if(!f) f=font_;
    int tw=0,th=0;
    TTF_SizeUTF8(f,s.c_str(),&tw,&th);
    text(s,x+(w-tw)/2,y+(32-th)/2,c,f);
}
void Renderer::circle(int cx,int cy,int radius,SDL_Color c,bool fill) {
    SDL_SetRenderDrawColor(r_,c.r,c.g,c.b,c.a);
    if(fill) {
        for(int dy=-radius;dy<=radius;dy++) {
            int dx=(int)std::sqrt(std::max(0,radius*radius-dy*dy));
            SDL_RenderDrawLine(r_,cx-dx,cy+dy,cx+dx,cy+dy);
        }
    } else {
        const int segments=48;
        for(int i=0;i<segments;i++) {
            double a1=2.0*M_PI*i/segments;
            double a2=2.0*M_PI*(i+1)/segments;
            SDL_RenderDrawLine(r_,cx+(int)(radius*std::cos(a1)),cy+(int)(radius*std::sin(a1)),cx+(int)(radius*std::cos(a2)),cy+(int)(radius*std::sin(a2)));
        }
    }
}

void Renderer::pulseCircle(int cx,int cy,int radius,SDL_Color c,double phase) {
    int r=radius+(int)(5.0*(0.5+0.5*std::sin(phase)));
    SDL_Color glow=c; glow.a=80;
    SDL_SetRenderDrawBlendMode(r_,SDL_BLENDMODE_BLEND);
    circle(cx,cy,r+8,glow,false);
    circle(cx,cy,r,c,true);
    SDL_SetRenderDrawBlendMode(r_,SDL_BLENDMODE_NONE);
}

void Renderer::movingDot(int x1,int y1,int x2,int y2,SDL_Color c,double t) {
    double p=(std::sin(t)+1.0)*0.5;
    int x=x1+(int)((x2-x1)*p);
    int y=y1+(int)((y2-y1)*p);
    pulseCircle(x,y,6,c,t*2.0);
}

void Renderer::line(int x1,int y1,int x2,int y2,SDL_Color c) {
    SDL_SetRenderDrawColor(r_,c.r,c.g,c.b,c.a);
    SDL_RenderDrawLine(r_,x1,y1,x2,y2);
}
void Renderer::arrow(int x1,int y1,int x2,int y2,SDL_Color c) {
    line(x1,y1,x2,y2,c);
    double a=std::atan2(y2-y1,x2-x1);
    int s=9;
    line(x2,y2,x2-(int)(s*std::cos(a-0.5)),y2-(int)(s*std::sin(a-0.5)),c);
    line(x2,y2,x2-(int)(s*std::cos(a+0.5)),y2-(int)(s*std::sin(a+0.5)),c);
}

void Renderer::conceptList(const Simulator& sim,int x,int y,int w,int h) {
    rect(x,y,w,h,panel);
    text("OOP CONCEPTS",x+16,y+14,white);
    const char* names[]={
        "Class & Object","Encapsulation","Inheritance","Polymorphism",
        "Abstraction","Constructor","Destructor","Virtual Function","Stack / Heap"
    };
    int yy=y+52;
    for(int i=0;i<9;i++) {
        Concept c=(Concept)i;
        bool selected=sim.simulation().concept==c;
        rect(x+10,yy,w-20,34,selected?SDL_Color{48,91,72,255}:panel2);
        text(names[i],x+20,yy+8,selected?white:muted,smallFont_);
        yy+=39;
    }
}

void Renderer::codePanel(const Simulator& sim,int x,int y,int w,int h) {
    rect(x,y,w,h,panel);
    text("SOURCE CODE",x+14,y+12,white);

    const auto& code=sim.simulation().code;
    const int lineH=23;
    const int firstY=y+45;
    const int visibleH=h-55;
    const int contentH=(int)code.size()*lineH;
    const int maxScroll=std::max(0,contentH-visibleH);
    codeScroll_=std::max(0,std::min(codeScroll_,maxScroll));

    SDL_Rect clip{x+8,y+38,w-16,h-45};
    SDL_RenderSetClipRect(r_,&clip);

    int active=-1;
    if(sim.currentStep()>0 && sim.currentStep()<=sim.totalSteps())
        active=sim.simulation().steps[sim.currentStep()-1].codeLine;

    int yy=firstY-codeScroll_;
    for(int i=0;i<(int)code.size();i++) {
        if(i+1==active) rect(x+8,yy-3,w-24,24,SDL_Color{57,74,62,255});
        text(std::to_string(i+1),x+14,yy,muted,smallFont_);
        text(code[i],x+48,yy,white,smallFont_);
        yy+=lineH;
    }

    SDL_RenderSetClipRect(r_,nullptr);

    if(maxScroll>0) {
        int trackX=x+w-9;
        int trackY=y+42;
        int trackH=h-52;
        rect(trackX,trackY,5,trackH,SDL_Color{48,54,65,255});
        int thumbH=std::max(25,(trackH*trackH)/contentH);
        int thumbY=trackY+(trackH-thumbH)*codeScroll_/maxScroll;
        rect(trackX,thumbY,5,thumbH,accent);
        text("Scroll source code",x+w-150,y+h-24,muted,smallFont_);
    }
}

void Renderer::diagram(const Simulator& sim,int x,int y,int w,int h, double animationTime) {
    rect(x,y,w,h,panel);
    text("VISUALIZATION",x+14,y+12,white);

    int cy=y+58;
    const auto& s=sim.simulation();
    int step=sim.currentStep();
    double t=animationTime;

    auto box=[&](const std::string& title,const std::string& body,int bx,int by,int bw,int bh,SDL_Color c){
        rect(bx,by,bw,bh,panel2);
        rect(bx,by,bw,4,c);
        centered(title,bx,by+8,bw,white,smallFont_);
        if(!body.empty()) centered(body,bx,by+35,bw,muted,smallFont_);
    };

    if(step==0) {
        text("Press STEP to introduce each part one-by-one.",x+45,cy+145,muted);
        text("RUN plays the same steps automatically.",x+75,cy+180,muted,smallFont_);
        return;
    }

    if(s.concept==Concept::ClassObject) {
        box("CLASS","Student",x+55,cy+10,180,110,blue);
        if(step>=2) {
            box("OBJECT","s1",x+310,cy+155,170,100,accent);
            arrow(x+235,cy+105,x+395,cy+155,accent);
            movingDot(x+235,cy+105,x+395,cy+155,accent,t*2.4);
        }
        if(step>=3) {
            text("name = Rahul",x+326,cy+190,white,smallFont_);
            text("rollNo = 18",x+326,cy+215,white,smallFont_);
            pulseCircle(x+395,cy+205,8,accent,t*3.0);
        }
        if(step>=4) {
            arrow(x+395,cy+255,x+395,cy+285,blue);
            movingDot(x+395,cy+255,x+395,cy+285,blue,t*3.0);
            text("display() → Rahul - 18",x+285,cy+305,accent,smallFont_);
        }
    }
    else if(s.concept==Concept::Encapsulation) {
        box("BankAccount","",x+170,cy+15,230,220,blue);
        text("PRIVATE",x+205,cy+58,red,smallFont_);
        text("balance = 5000",x+205,cy+85,white,smallFont_);
        if(step>=2) {
            text("PUBLIC",x+205,cy+125,accent,smallFont_);
            text("deposit()",x+205,cy+153,white,smallFont_);
            text("getBalance()",x+205,cy+180,white,smallFont_);
            pulseCircle(x+360,cy+85,7,red,t*2.2);
            text("Direct private access",x+175,cy+255,red,smallFont_);
            text("ACCESS DENIED",x+205,cy+280,red);
        }
        if(step>=3) {
            arrow(x+400,cy+150,x+485,cy+150,accent);
            movingDot(x+400,cy+150,x+485,cy+150,accent,t*3.0);
            text("deposit(1000)",x+400,cy+125,accent,smallFont_);
        }
        if(step>=4) {
            text("Balance = 6000",x+205,cy+310,accent);
        }
    }
    else if(s.concept==Concept::Inheritance) {
        box("BASE CLASS","Vehicle",x+190,cy+10,190,85,blue);
        if(step>=2) {
            box("DERIVED CLASS","Car",x+190,cy+180,190,85,accent);
            arrow(x+285,cy+95,x+285,cy+180,accent);
            movingDot(x+285,cy+95,x+285,cy+180,accent,t*2.0);
            text("inherits",x+300,cy+130,muted,smallFont_);
        }
        if(step>=3) {
            arrow(x+190,cy+220,x+115,cy+285,blue);
            movingDot(x+190,cy+220,x+115,cy+285,blue,t*2.5);
            text("start()",x+75,cy+300,white,smallFont_);
        }
        if(step>=4) {
            arrow(x+380,cy+220,x+455,cy+285,accent);
            movingDot(x+380,cy+220,x+455,cy+285,accent,t*2.5+1.0);
            text("drive()",x+425,cy+300,white,smallFont_);
        }
    }
    else if(s.concept==Concept::Polymorphism || s.concept==Concept::VirtualFunction) {
        box("Animal","virtual sound()",x+190,cy,210,85,blue);
        if(step>=2) {
            box("Dog","sound()",x+80,cy+190,180,85,accent);
            box("Cat","sound()",x+330,cy+190,180,85,yellow);
            arrow(x+245,cy+85,x+170,cy+190,accent);
            arrow(x+345,cy+85,x+420,cy+190,yellow);
            movingDot(x+245,cy+85,x+170,cy+190,accent,t*2.1);
            movingDot(x+345,cy+85,x+420,cy+190,yellow,t*2.1+1.5);
        }
        if(step>=3) {
            box("BASE POINTER","Animal* a",x+190,cy+115,210,60,cyan);
            arrow(x+295,cy+175,x+170,cy+190,cyan);
            movingDot(x+295,cy+175,x+170,cy+190,cyan,t*2.6);
        }
        if(step>=4) {
            text("a->sound()",x+225,cy+295,white);
            text("BARK!",x+125,cy+325,accent);
            text("one interface → different behavior",x+210,cy+325,white,smallFont_);
        }
    }
    else if(s.concept==Concept::Abstraction) {
        box("PUBLIC INTERFACE","start()",x+190,cy,210,80,blue);
        if(step>=2) {
            box("HIDDEN IMPLEMENTATION","Engine • Ignition • Sensors",x+115,cy+140,360,105,accent);
            arrow(x+295,cy+80,x+295,cy+140,accent);
            movingDot(x+295,cy+80,x+295,cy+140,accent,t*2.0);
        }
        if(step>=3) text("Implementation details stay hidden.",x+165,cy+270,muted,smallFont_);
        if(step>=4) {
            arrow(x+295,cy+245,x+295,cy+295,blue);
            movingDot(x+295,cy+245,x+295,cy+295,blue,t*2.8);
            text("User calls start()",x+235,cy+315,accent,smallFont_);
        }
    }
    else if(s.concept==Concept::Constructor) {
        box("NEW OBJECT","Student s1",x+190,cy+10,210,80,blue);
        if(step>=2) {
            arrow(x+295,cy+90,x+295,cy+135,accent);
            movingDot(x+295,cy+90,x+295,cy+135,accent,t*2.0);
            box("CONSTRUCTOR","Student()",x+190,cy+140,210,80,accent);
        }
        if(step>=3) {
            arrow(x+295,cy+220,x+295,cy+265,accent);
            movingDot(x+295,cy+220,x+295,cy+265,accent,t*2.0+1.0);
            box("INITIALIZED","name = Rahul",x+190,cy+270,210,70,blue);
        }
        if(step>=4) text("Object ready for use",x+225,cy+355,accent,smallFont_);
    }
    else if(s.concept==Concept::Destructor) {
        box("OBJECT ALIVE","Student s1",x+190,cy+10,210,80,blue);
        if(step>=2) {
            arrow(x+295,cy+90,x+295,cy+135,yellow);
            movingDot(x+295,cy+90,x+295,cy+135,yellow,t*2.0);
            box("SCOPE ENDS","object lifetime ends",x+190,cy+140,210,80,yellow);
        }
        if(step>=3) {
            arrow(x+295,cy+220,x+295,cy+265,red);
            movingDot(x+295,cy+220,x+295,cy+265,red,t*2.0+1.0);
            box("DESTRUCTOR","~Student()",x+190,cy+270,210,70,red);
        }
        if(step>=4) text("Object destroyed",x+240,cy+355,red,smallFont_);
    }
    else if(s.concept==Concept::Memory) {
        box("STACK","s1",x+70,cy+20,180,170,blue);
        if(step>=2) box("HEAP","Student object",x+330,cy+20,210,170,accent);
        if(step>=3) {
            arrow(x+250,cy+105,x+330,cy+105,accent);
            movingDot(x+250,cy+105,x+330,cy+105,accent,t*2.0);
            text("p",x+275,cy+80,white,smallFont_);
            pulseCircle(x+435,cy+105,10,accent,t*2.5);
        }
        if(step>=4) {
            arrow(x+435,cy+190,x+435,cy+235,red);
            movingDot(x+435,cy+190,x+435,cy+235,red,t*2.5);
            text("delete p → memory released",x+315,cy+260,red,smallFont_);
        }
    }
}

void Renderer::infoPanel(const Simulator& sim,int x,int y,int w,int h) {
    rect(x,y,w,h,panel);
    text(sim.simulation().name,x+14,y+10,accent);
    text(sim.simulation().definition,x+14,y+42,muted,smallFont_);
    int step=sim.currentStep();
    if(step>0) {
        const auto& st=sim.simulation().steps[step-1];
        text("CURRENT STEP",x+14,y+78,yellow,smallFont_);
        text(st.title,x+14,y+101,white);
        text(st.description,x+14,y+130,muted,smallFont_);
        if(!st.output.empty()) text("Output: "+st.output,x+14,y+160,accent,smallFont_);
    } else {
        text("Press STEP or RUN to start the simulation.",x+14,y+95,muted,smallFont_);
    }
}

void Renderer::bottomBar(const Simulator& sim,int x,int y,int w,int h) {
    rect(x,y,w,h,panel);
    const char* b[]={"RUN","STEP","PAUSE","RESET"};
    int bx=x+15;
    for(int i=0;i<4;i++) {
        SDL_Color c=(i==0 && sim.isRunning())?accent:panel2;
        rect(bx,y+10,92,36,c);
        centered(b[i],bx,y+10,92,white,smallFont_);
        bx+=104;
    }
    text("Step "+std::to_string(sim.currentStep())+" / "+std::to_string(sim.totalSteps()),x+w-180,y+20,muted,smallFont_);
}

void Renderer::draw(const Simulator& sim,int width,int height, double animationTime) {
    SDL_SetRenderDrawColor(r_,bg.r,bg.g,bg.b,bg.a);
    SDL_RenderClear(r_);

    rect(0,0,width,58,SDL_Color{24,29,37,255});
    text("OOP VISUALIZER",22,15,white);
    text("Interactive C++ OOP learning simulator",225,20,muted,smallFont_);

    int sidebarW=190;
    int gap=10;
    int rightX=sidebarW+gap;
    int rightW=width-rightX-10;
    int topY=68;

    conceptList(sim,0,topY,sidebarW,height-topY-76);

    int bottomH=66;
    int infoH=205;
    int diagramH=std::max(300,height-topY-bottomH-infoH-20);
    diagram(sim,rightX,topY,rightW,diagramH,animationTime);

    int codeW=(rightW*58)/100;
    int infoW=rightW-codeW-gap;
    int lowerY=topY+diagramH+10;
    int lowerH=height-bottomH-lowerY;
    codePanel(sim,rightX,lowerY,codeW,lowerH);
    infoPanel(sim,rightX+codeW+gap,lowerY,infoW,lowerH);

    bottomBar(sim,0,height-66,width,66);
    SDL_RenderPresent(r_);
}

void Renderer::handleWheel(int mx,int my,int delta,int width,int height) {
    int sidebarW=190;
    int gap=10;
    int rightX=sidebarW+gap;
    int rightW=width-rightX-10;
    int topY=68;
    int bottomH=66;
    int infoH=205;
    int diagramH=std::max(300,height-topY-bottomH-infoH-20);
    int lowerY=topY+diagramH+10;
    int codeW=(rightW*58)/100;
    int lowerH=height-bottomH-lowerY;

    if(mx>=rightX && mx<rightX+codeW && my>=lowerY && my<lowerY+lowerH) {
        codeScroll_ -= delta*30;
        if(codeScroll_<0) codeScroll_=0;
    }
}

bool Renderer::handleClick(int mx,int my,int width,int height,Simulator& sim) {
    int sidebarW=190, topY=68;
    if(mx>=0 && mx<sidebarW && my>=topY && my<height-76) {
        int idx=(my-(topY+52))/39;
        if(idx>=0 && idx<9) {
            sim.select((Concept)idx);
            codeScroll_=0;
            return true;
        }
    }

    if(my>=height-66) {
        if(mx>=15 && mx<107) { sim.runToggle(); return true; }
        if(mx>=119 && mx<211) { sim.step(); return true; }
        if(mx>=223 && mx<315) { sim.pause(); return true; }
        if(mx>=327 && mx<419) { sim.reset(); return true; }
    }
    return false;
}
