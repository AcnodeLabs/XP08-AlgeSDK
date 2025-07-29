// AlgeSDK.ads.h // The high level interface to AlgeSDK in conformance with Ada Specifications
#ifndef ALGESDK_ADS_H
#define ALGESDK_ADS_H
#include <math.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <list>

#define deprecatedfunction

#ifdef __is_target_os
#ifndef MACOSX
//#define NO_BOX2D
#endif
#endif



#ifndef NO_BOX2D //Box2D is to be built by premake which does not support
#include <Box2D/Box2D.h>
#endif

using namespace std;
#define GLfloat float
#include "../CCommands.h"
#include "../../Classlets/com/acnodelabs/funkit/CAnimator.hpp"
#include "../../Base/camera.h"
#include "../CBaseV1_2.h"



const float FACTOR_RADIANS_DEGREES = 57.295779513082;
void inline aluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                      GLfloat centerx, GLfloat centery, GLfloat centerz,
                      GLfloat upx, GLfloat upy, GLfloat upz);

class i2 {
public:
	int x, y;
	void clear() { x = 0; y = 0; }
	i2() { clear(); };
	
	i2(int mx, int my) { x = mx; y = my; }
    void CopyFrom(i2 p) { x = p.x; y = p.y; }
    i2 half() { return i2(x / 2, y / 2); }
    i2 mult(float factor) { return i2(x * factor, y * factor); }
	i2 twice() { return i2(x * 2, y * 2); }
    i2 flip() {return i2(y,x);}
	bool isZero() { return (x == 0 && y == 0); }
    
};


class f2 {
public:
	float x, y;
	void clear() { x = 0; y = 0; }
	f2() { clear(); };
	f2(float mx, float my) { x = mx; y = my; }
	void CopyFrom(f2 p) { x = float(p.x); y = float(p.y); }
    f2 half() { return f2(x / 2, y / 2); }
    f2 twice() { return f2(x * 2, y * 2); }
    f2 flip() {return f2(y,x);}
    i2 toi2() {return i2(x,y);}
};

class f3 {
public:
    float x,y,z;
	char xyz[64] = { 0 };
	void clear() { x = 0; y = 0; z = 0; }
	f3(float mx, float my, float mz) { x = mx; y = my; z = mz; }
    void set(float mx, float my, float mz) { x = mx; y = my; z = mz; }
    void set(f3 p) { x = p.x; y = p.y; z = p.z; }
	f3(i2 m) { x = m.x; y = m.y; z = 0; }
	string str(string fmt) {
    snprintf(xyz, 64, fmt.c_str(), x, y, z);
    //sprintf(xyz, fmt.c_str(), x, y, z);
		return string(xyz);
	}
	f2 xy() {return f2( x,y ); }
    i2 ixy() { return i2((int)x, (int)y); }
	f3* ref() { return this; }

    f3() {clear();};
};


struct CRect {
public:
    float Top, Bottom, Left, Right;
    CRect() {};
    CRect(float _top, float _bottom, float _left, float _right) {
        Top = _top; Bottom = _bottom; Left = _left; Right = _right;
    }
    
 //   CRect objectToScreen() {}
 //   CRect screenToScreen() {}
    
    CRect scaledRect(CRect s) {
        CRect ret;
        float width = abs(Right-Left);
        float height = abs(Top-Bottom);
        float cX = width / 2.0;
        float cY = height / 2.0;
        
        float s_width = abs(s.Right-s.Left);
        float s_height = abs(s.Top-s.Bottom);
        
        float x_scale_factor = s_width / width;
        float y_scale_factor = s_height / height;
        
        float new_width = width * x_scale_factor;
        float new_height = height * y_scale_factor;
        
        float cnX = cX / width * new_width;
        float cnY = cY / height * new_height;
        
        ret.Left = cnX - (new_width / 2.);
        ret.Right = cnX + (new_width / 2.);
        
        ret.Top = cnY - (new_height / 2.);
        ret.Bottom = cnY + (new_height / 2.);
        return ret;
    }
    
    //updated top is 0 and bottom is max
    static bool PTInRect(float x1, float y1, CRect aRect, string uuid = "") {
        //((y1 >= aRect.Top) && (y1<aRect.Bottom) && (x1 >= aRect.Left) && (x1<aRect.Right));
        bool ret1 = (y1 >= aRect.Top);
        bool ret2 = (y1 <  aRect.Bottom);// && (x1 >= aRect.Left) && (x1<aRect.Right));
        bool ret3 = (x1 >= aRect.Left);// && (x1<aRect.Right));
        bool ret4 = (x1 <  aRect.Right);
        return (ret1 && ret2 && ret3 && ret4);
    }
    // Update required as now in 2d  top is 0 and bottom is max
    static bool OverlapRect(CRect Rect1, CRect Rect2) {
        bool result = false;
        float cx = (Rect1.Left + Rect1.Right) / 2.f;
        float cy = (Rect1.Top + Rect1.Bottom) / 2.f;
        
        result = PTInRect(cx, cy, Rect2, "OverLapRect");
        return result;
    }
     // Update required as now in 2d  top is 0 and bottom is max
    static bool RectRectCollision(CRect Rect1, CRect Rect2) {
        return  !(
                  (Rect1.Bottom < Rect2.Top) ||
                  (Rect1.Top > Rect2.Bottom) ||
                  (Rect1.Left > Rect2.Right) ||
                  (Rect1.Right < Rect2.Left));
    }
    
	bool improper() {
		bool ret;
		int w = Right - Left;
		int h = Bottom - Top;
		ret = !(w > 0 && h > 0);
		return ret;
	}

	std::string toStr() {
		ostringstream oss;
		int w = Right - Left;
		int h = Bottom - Top;
		int cx = Left + w / 2;
		int cy = Top + h / 2;
		oss << "T" << Top << " B" << Bottom << " H" << h << " L" << Left << " R" << Right << " W" << w << " X" << cx << " Y" << cy;
		return oss.str();
	}

};

enum JuiceTypes {
    NIL = 0,
	JUICE_ROTZ = 1,
	JUICE_SCALE_OUT,
	JUICE_SCALE_IN,
	JUICE_FLY_OUT,
	JUICE_FLY_IN,
	JUICE_ROTY,
    JUICE_ROTX,
	JUICE_PULSATE,
	JUICE_PULSATE_FULLY,
	JUICE_ROTZ_PULSATE,
	JUICE_ROTXYZ,
	JUICE_ROTXYZ_PULSATE_FULLY,
	JUICE_DIE,
	JUICE_DIE_TEMP,
    JUICE_FLICKER,
	JUICES_CANCEL,
	JUICES_END
};

string JuiceName(int j) {
	if (j == 0) return "NIL";
	if (j == JUICE_ROTZ) return "JUICE_ROTZ";
	if (j == JUICE_SCALE_IN) return "JUICE_SCALE_IN";
	if (j == JUICE_SCALE_OUT) return "JUICE_SCALE_OUT";
	if (j == JUICE_FLY_IN) return "JUICE_FLY_IN";
	if (j == JUICE_FLY_OUT) return "JUICE_FLY_OUT";
	if (j == JUICE_ROTY) return "JUICE_ROTY";
    if (j == JUICE_ROTX) return "JUICE_ROTX";
	if (j == JUICE_PULSATE) return "JUICE_PULSATE";
	if (j == JUICE_PULSATE_FULLY) return "JUICE_PULSATE_FULLY";
	if (j == JUICE_ROTZ_PULSATE) return "JUICE_ROTZ_PULSATE";
	if (j == JUICE_ROTXYZ) return "JUICE_ROTXYZ";
	if (j == JUICE_ROTXYZ_PULSATE_FULLY) return "JUICE_ROTXYZ_PULSATE_FULLY";
	if (j == JUICE_DIE) return "JUICE_DIE";
	if (j == JUICE_DIE_TEMP) return "JUICE_DIE_TEMP";
    if (j == JUICE_FLICKER) return "JUICE_FLICKER";
	if (j == JUICES_CANCEL) return "JUICE_CANCEL";
	if (j == JUICES_END) return "JUICE_END";
	return "Wrong ID " + std::to_string(j);
}

class PosRotScale {

public:
    f3 pos;
    f3 rot;
        float m_width = 1.2345f;
        float m_height = 1.2345f;



	PosRotScale() {
		color.x = 1.;
		color.y = 1.;
		color.z = 1.;
	}

	PosRotScale(f3 pos, f3 rot, float scale = 1.0) {
		PosRotScale();
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}

    string toString() {
        char sz[128];
        sprintf(sz, "sc:%.2f %.fx%.f @%.f,%.f", this->originalScale, this->m_width, m_height, pos.x, pos.y
        );
        return this->UUID + string(sz);
    }

    bool named(string name) {
        return (UUID.find(name) != string::npos);
    }


	f3 color;

	float scale;
	bool hidden = false;
	int JuiceType;
	float JuiceSpeed = 1.;
	float JuiceDuration = 3;
    bool touchable = true;
	double juice_sine_angle = 0.0;
	float originalScale = 1.;
    float originalAspect = 1.78;
    float debugUseOnly = 0.;

#ifndef NO_BOX2D
	 b2Body* physBodyPtr = nullptr;
#endif
	 int m_touchedX;
	 int m_touchedY;
	 string UUID;

     string GetName() {
         return UUID;
     }

     short instanceNo;

	 CRect getOwnRect(string name = "") {
         
         int my_height = m_height;
         int my_width  = m_width;

         float sf = 2.0;// *this->originalScale;
	    CRect own(
            pos.y - my_height / sf,
            pos.y + my_height / sf, 
            pos.x - my_width / sf, 
            pos.x + my_width / sf);
		 return own;
	 }

    bool Visible() {
        return !hidden;
    }
    
	 void SetJuiceScaleIn() {
		 this->scale = 0.1;
		 this->JuiceType = JuiceTypes::JUICE_SCALE_IN;
	 }

    void CopyFrom(PosRotScale* o) {
        pos.x = o->pos.x;
        pos.y = o->pos.y;
        pos.z = o->pos.z;
        rot.x = o->rot.x;
        rot.y = o->rot.y;
        rot.z = o->rot.z;
        scale = o->scale;
        hidden = o->hidden;
        JuiceType = o->JuiceType;
        JuiceSpeed = o->JuiceSpeed;
        originalScale = o->originalScale;
        originalAspect = o->originalAspect;
		m_width = o->m_width;
		m_height = o->m_height;
#ifndef NO_BOX2D
		physBodyPtr = o->physBodyPtr;
#endif
		UUID = o->UUID;
    }

    void StraigthenUp() {
#ifndef NO_BOX2D
        if (physBodyPtr)
            physBodyPtr->SetTransform(physBodyPtr->GetPosition(), 0);
#endif
    }

	void Impulse(f2 up) {
#ifndef NO_BOX2D
		static b2Vec2 bUp;
		bUp.x = up.x;
		bUp.y = up.y;
		if (physBodyPtr) physBodyPtr->ApplyLinearImpulse(bUp, physBodyPtr->GetWorldCenter(), true);
#endif
	}

    void Thrust(f2 up) {
#ifndef NO_BOX2D
        static b2Vec2 bUp;
        bUp.x = up.x;
        bUp.y = up.y;
        if (physBodyPtr) physBodyPtr->ApplyForceToCenter(bUp, true);
#endif
  }
    
    void Torque(int angle) {
#ifndef NO_BOX2D
        if (physBodyPtr) physBodyPtr->ApplyTorque(angle, true);
#endif
    }
    
    /*
    NIL = 0,
    JUICE_ROTZ = 1,
	JUICE_SCALE_OUT,
	JUICE_SCALE_IN,
	JUICE_FLY_OUT,
	JUICE_FLY_IN,
	JUICE_ROTY,
    JUICE_ROTX,
	JUICE_PULSATE,
	JUICE_PULSATE_FULLY,
	JUICE_ROTZ_PULSATE,
	JUICE_ROTXYZ,
	JUICE_ROTXYZ_PULSATE_FULLY,
	JUICE_DIE,
	JUICE_DIE_TEMP,
    JUICE_FLICKER,
	JUICES_CANCEL,
	JUICES_END
    */

    void JuiceCombo(int* curr) {
        const char* items[] = { "NIL","JUICE_ROTZ", "JUICE_SCALE_OUT", "JUICE_SCALE_IN", "JUICE_FLY_OUT", "JUICE_FLY_IN", "JUICE_ROTY", "JUICE_ROTY", "JUICE_ROTX", "JUICE_PULSATE", "JUICE_PULSATE_FULLY", "JUICE_ROTZ_PULSATE", "JUICE_ROTXYZ", "JUICE_ROTXYZ_PULSATE_FULLY", "JUICE_DIE", "JUICE_DIE_TEMP", "JUICE_FLICKER", "JUICES_CANCEL", "JUICES_END" };
        static const char* current_item = NULL;
#ifndef NO_IMGUI

        if (ImGui::BeginCombo("##combo", items[*curr])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected)) {
                    current_item = items[n];
                    *curr = n;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                } // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

#endif // !NO_IMGUI

    }

    void ShowVitals() {
      
     //   GuiStarts();// needed because we are out of RenderGui function
        CRect r = getOwnRect();
        f3 pos = GetPos();
        string name = UUID + ".";//Name should noe be empty
        i2 dif(pos.ixy().x - r.Left, pos.ixy().y - r.Top);
        
#ifndef NO_IMGUI        
        ImGui::SetNextWindowSize(ImVec2(250, 120));
        //ImGui::SetNextWindowPos(ImVec2(r.Left, r.Top));
        ImGui::SetNextWindowBgAlpha(0.5);
      //  ImGui::SetNextWindowCollapsed(true);
        ImGui::Begin(name.c_str());// , ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Pos = {%.1f,%.1f}", pos.x, pos.y);
        ImGui::Text("Juice=%d", this->JuiceType); 
        ImGui::SameLine();
        JuiceCombo(&this->JuiceType); 
        ImGui::SliderFloat("JuiceSpeed", &this->JuiceSpeed, -5.,5.);
        ImGui::Text("Rect = {%.1f,%.1f,%.1f,%.1f}", r.Top, r.Bottom, r.Left, r.Right);
        //
        ImGui::End();
#endif
    //    GuiEnds();
    }
    void SetPos(double posx, double posy, double posz = 0.0) {
        this->pos.x = posx;
        this->pos.y = posy;
        this->pos.z = posz;
    }
    
    void SetDim(int w, int h) {
        m_width = w; m_height = h;
    }


    void SetDim(f2 wh) {
        m_width = wh.x; m_height = wh.y;
    }

    f2 GetDim() {
        return f2(m_width, m_height);
    }
    void SetPos(f3 pos) {
        this->pos = pos;
    }
    
    f3 GetPos() {
#ifndef NO_BOX2D

        if (this && this->physBodyPtr) {
            b2Vec2 pxy = this->physBodyPtr->GetPosition();
            pos.x = pxy.x * 100;//P2S
            pos.y = pxy.y * 100;//P2S
        }
#endif // !NO_BOX2D

        return this->pos;
    }

    void SetRot(f3 rot) {
        this->rot = rot;
    }

    void SetRotZ(float angleDeg) {
        this->rot.z = angleDeg;
    }


    //Impl To be verified
    f3 GetRot() {
#ifndef NO_BOX2D
        if (this && this->physBodyPtr) {
            b2Transform pxy = this->physBodyPtr->GetTransform();
            rot.x = pxy.p.x;// *100;//P2S
            rot.y = pxy.p.y;// *100;//P2S
            //
        }
#endif
        return this->rot;
    }
};

class Serializable : public PosRotScale {
    
    public:

    //can be used externally for Appends
    void readin(FILE *f) {
        f3 pos, rot;
        fread((void*)&pos,sizeof(pos),1, f);
        SetPos(pos);
        fread((void*)&rot,sizeof(rot),1, f);
        SetRot(rot);
        fread((void*)&scale,sizeof(scale),1, f);
    }
    
    void writeout(FILE *f) {
        f3 pos = GetPos();
        f3 rot = GetRot();
        fwrite((void*)&pos,sizeof(pos),1, f);
        fwrite((void*)&rot,sizeof(rot),1, f);
        fwrite((void*)&scale,sizeof(scale),1, f);
    }
    
    void SetSerializeTag(const char* UUID) {
        this->UUID = string(UUID);
     //   return;
        if (this->UUID.size()>0) {
            FILE* f = fopen(this->UUID.c_str(), "rb");
            if (f) {
              //  readin(f);
                fclose(f);
            }
        }
    }
    
    ~Serializable() {
        if (UUID.size()>0) {
            FILE* f = fopen(UUID.c_str(), "wb");
            if (f) {
                writeout(f);
                fclose(f);
                printf("\tSerialized: %s", UUID.c_str());
            }
        }
    }
};

extern class App game;

class GameObject : public Serializable {
    PosRotScale m_prs;

public:
    CRect m_rect;
    bool m_touched;//deprecated   use touched_bodies instead
	
	bool m_actionComplete = false;
    
    int m_touchedX;//deprecated
    int m_touchedY;//deprecated
	bool hud = false;
    //Add PRS to multiple drawing of same model
    //use getInstancePtr to access instances
    vector<PosRotScale> prsInstances;
    int modelId;
    ResourceInf* resInf;
    short custom_type;
    GameObject* parent;
    std::list<GameObject*> children;
    short instanceBiengRendered;
    bool rotatefirst;
    
    string Text;
    // float Scale;
    bool billboard;
    PosRotScale desirable;
    
    bool applyTopLeftCorrectionWRTorigin;
    
    static i2 windowSize;
    
    CAnimator animPos, animRot, animScale;
    
#ifndef NO_BOX2D
    b2Body * phys_body;
#endif
    
    void Show() {hidden = false;}
    void Hide() {
		hidden = true;
	}
    void HideFor(GameObject* next) {
        Hide();
		next->Show();
    }



    inline bool isMovingDown2D() {
#ifndef NO_BOX2D
        if (!physBodyPtr) return false;
        return physBodyPtr->GetLinearVelocity().y > 0.0;
#else
		return false;
#endif // !NO_BOX2D
    }
    
    inline bool isMovingUp2D() {
#ifndef NO_BOX2D
        if (!physBodyPtr) return false;
       return physBodyPtr->GetLinearVelocity().y < 0.0;
#else
return false;
#endif // !NO_BOX2D
    }
     
    bool isStationary() {
#ifndef NO_BOX2D
        if (!physBodyPtr) return true;
        float yy = physBodyPtr->GetLinearVelocity().y;
        return yy == 0;
#else
return false;
#endif // !NO_BOX2D
    }

    PosRotScale GetPrs() { return m_prs; }

    float vy() {
#ifndef NO_BOX2D
        if (!physBodyPtr) return 0.0;
        return physBodyPtr->GetLinearVelocity().y;
#else
		return 0.0;
#endif // !NO_BOX2D
    }

    void SetBounds(float fWidth, float fHeight, string name= "") {
        m_width = fWidth;
        m_height = fHeight;
        m_prs.m_width = m_width;
        m_prs.m_height = m_height;
        m_prs.SetDim(fWidth, fHeight);
    }

    int texWidth;

    void setTexWidth(int tw) {
        texWidth = tw;
    }

    int getTexWidth() {
        return  texWidth;
    }

	bool is(GameObject& other) {
		if (this->modelId == -1) {
			return (this->UUID == other.UUID);
		} else {
			return (this->modelId == other.modelId);
		}
	}

    bool isOneOf(vector<GameObject*> other) {
        bool ret = false;
        for (auto o : other) {
            ret = ret || (this->modelId == o->modelId);
        }
        return ret;
    }
    
    void JuiceNext() {
        JuiceType ++;
        if (JuiceType == JuiceTypes::JUICES_END)
            JuiceType = 0;
    } 

    //Use JuiceNext instead
    void deprecatedfunction NextJuice() {
        JuiceNext();
    }

    void JuiceReset() {
        JuiceType = 0;
        JuiceSpeed = 1.;
        JuiceDuration = 3;
        juice_sine_angle = 0.0;
    }
    
    bool wasTouched() {
		if (hidden) return false;
		bool m_touched_act = m_touched;
		m_touched = false;
        if (m_touched_act) {
			return true;
		} else return false;
    }
    
	bool actionComplete() {
		if (hidden) return false;
		if (m_actionComplete) { m_actionComplete = false; return true; }
		else return false;
	}

    f2 posTouched() {
        return f2(m_touchedX, m_touchedY);
    }
    
    void Update(float dt) {
        
		if (animPos.active) {
			animPos.Step(dt);
            m_prs.SetPos(f3(mPos[0], mPos[1], mPos[2]));
        }
        if (animRot.active) {
            animRot.Step(dt);
            m_prs.SetRot(f3(mRot[0], mRot[1], mRot[2]));			
        }
        if (animScale.active) {
            animScale.Step(dt);
            scale = mScale[0];//3d?
			m_actionComplete = false;
        }

		m_actionComplete = (animPos.r>=1 && animRot.r>=1 && animRot.r>=1);
	
#ifndef NO_BOX2D
        if (phys_body && phys_body->GetType()==b2_dynamicBody) {
            b2Vec2 position = phys_body->GetPosition();
            float32 angle = phys_body->GetAngle();
            m_prs.SetPos(f3(position.x,position.y,0));
            m_prs.SetRotZ(angle * 57.272727);
        }
#endif
        f3 pos = m_prs.GetPos();
        f2 dim = m_prs.GetDim();
        m_rect.Top = pos.y - dim.y / 2.0;
        m_rect.Bottom = pos.y + dim.y / 2.0 ;
        m_rect.Left = pos.x - dim.x / 2.0;
        m_rect.Right = pos.x + dim.x / 2.0;
    }
    
    void o2vf(f3* o, float* v3) {
        v3[0] = o->x; v3[1] = o->y; v3[2] = o->z;
    }
    
    
    float tgtPos[3];
    float tgtRot[3];
    float mPos[3];
    float mRot[3];
    float mScale[3];
    float tgtScale[3];
    
    void transitionTof2(f2 pos, float speed = 100.0, short type = 0) {
        PosRotScale tgt;
        tgt.CopyFrom(this);
        tgt.SetPos(f3(pos.x,pos.y,0));
        transitionTo(tgt, speed, type);
		m_actionComplete = false;
    }
    
    void copyTextureFrom(GameObject* tgt) {
        
    }

    void transitionTo(PosRotScale tgt, float speed = 100., short type = 0) {
        f3 tgt_pos = tgt.GetPos();
        f3 tgt_rot = tgt.GetRot();
        o2vf(&tgt_pos, tgtPos);
        o2vf(&tgt_rot, tgtRot);
        f3 m_prs_pos, m_prs_rot;
        o2vf(&m_prs_pos, mPos);
        o2vf(&m_prs_rot, mRot);
        mScale[0] = mScale[1] = mScale[2] = scale;
        tgtScale[0]=tgtScale[1]=tgtScale[2]= tgt.scale;
        animPos.Reset(mPos, tgtPos, speed, type);
        animRot.Reset(mRot, tgtRot, speed, type);
        animScale.Reset(mScale, tgtScale, speed, type);
    }
    
    void Clone(GameObject* clone) {
        clone->m_prs.SetPos(m_prs.GetPos());
        clone->m_prs.SetRot(m_prs.GetRot());
        //   clone->Scale = Scale;
        clone->scale = scale;
        clone->resInf = resInf;
        clone->custom_type = custom_type;
        clone->modelId = modelId;
        //clone->children = children;
        clone->hidden = hidden;
        clone->rotatefirst = rotatefirst;
        clone->parent = parent;
        clone->billboard = billboard;
        clone->m_rect = m_rect;
        f2 dim = m_prs.GetDim();
        clone->m_prs.SetDim(dim.x,dim.y);
    }
    
    void AddChild(GameObject* child){
        children.push_back(child);
        child->parent = this;
    }
    PosRotScale* AddInstance(PosRotScale &prs, short instanceNo = -1) {
        if (prs.scale <= 0) prs.scale = 1.0;
        prs.originalScale = prs.scale;
       //?? prs.originalAspect = prs.originalAspect
		prs.UUID = this->UUID + to_string(instanceNo);
        prsInstances.push_back(prs);
        prs.instanceNo = (short)prsInstances.size()-1;
        return &prs;
    }

    
    PosRotScale prst;
    
    PosRotScale*  AddInstance(f2 pos) {
        prst.SetPos(f3(pos.x, pos.y, 0));
        prst.scale = 1.0;// this->scale;
        
        prst.JuiceType = this->JuiceType;
        prst.JuiceSpeed = this->JuiceSpeed;
        prst.originalScale = this->originalScale;
        prst.originalAspect = this->originalAspect;
        f2 dim = this->m_prs.GetDim();
        prst.SetDim(dim.x, dim.y);
        prsInstances.push_back(prst);
        prst.instanceNo =(short) prsInstances.size()-1;
		return &prsInstances.back();
    }
    
    //getInstance(-1) returns main object instances are at 0..n-1
    PosRotScale* getInstancePtr(int n) {
        if (n < 0 || n >= int(prsInstances.size())) 
            return ((PosRotScale*)this);
        return &prsInstances.at(n);
    }
    
   
    GameObject() {
        m_prs.SetPos(f3(0,0,0));
        m_prs.SetRot(f3(0,0,0));
        prsInstances.clear();
        modelId = -1;
        scale = 1.;
        resInf = nullptr;
        custom_type = -1;
        children.clear();
        hidden = false;
        parent = nullptr;
        rotatefirst = false;
        UUID = "";
        //    Scale = 1.0f;
        billboard = false;
        applyTopLeftCorrectionWRTorigin = false;
        m_touched = false;
		resInf = nullptr;
    }
    
    string Name() {
      //  printf(UUID.c_str());
		if (UUID.size()) return UUID;
        if (Text.size()) return Text;
        if (!resInf) return "Undefined";
       // if (resInf->name.size()) return resInf->name;
        if (resInf) if (resInf->alx.size()) return resInf->alx;
        return "Undefined";
    }
    
    ~GameObject() {
        
    }
    
};
i2 GameObject::windowSize;


class Camera : public GameObject, public CFpsCamera {
	
public:
    short mode;
	int windowWidth;//for 2D ortho setup
	int windowHeight;
    int orthoType;
    
    enum {
        CAM_MODE_NULL,
        CAM_MODE_FPS,
        CAM_MODE_LOOKAT,
		CAM_MODE_2D,
        CAM_MODE_CHASE,
        CAM_MODE_FLY,
        CAM_MODE_LAST //ALL MODES AFTER THIS WILL BE IGNORED
    } camModes;
    
	void SetMode(int MODE) {
		this->mode = MODE;
	}
	
	int GetMode() {
		return this->mode;
	}

	Camera(int MODE) {
		SetMode(MODE);
     	Position.z = 10;
		resInf = new ResourceInf();
        resInf->Set("Camera", "", "",1);
    }
    
	f3 getPos() {
		return f3(Position.x, Position.y, Position.z);
	}

	f3 getRot() {
		return f3(RotatedX, RotatedY, RotatedZ);
	}


    Camera()  {
        SetMode(CAM_MODE_NULL);
    }
    
    ~Camera() {
        delete resInf;
    }
    
	inline void MoveAhead(float d) { /*pos.z += d; Position.z = pos.z;*/ }
	inline void MoveBack(float d) { /*pos.z-= d; Position.z = pos.z;*/
    }
    inline void MoveRight(float d) {/*pos.x+=d; Position.x = pos.x;*/
    }
    inline void MoveLeft(float d) {/*pos.x-=d; Position.x = pos.x;*/
    }
    inline void MoveDown(float d) {/*pos.y-=d; Position.y = pos.y;*/
    }
    inline void MoveUp(float d) {/*pos.y+=d; Position.y = pos.y;*/
    }
    
	inline void RollRight(float d) { /*rot.z += d; RotatedZ = rot.z;*/ }
    inline void RollLeft(float d) {/*rot.z-=d; RotatedZ = rot.z;*/
    }
    inline void PitchUp(float d) {/*rot.x-=d; RotatedX = rot.x;*/
    }
    inline void PitchDown(float d) {/*rot.x+=d; RotatedX = rot.x;*/
    }
    inline void YawRight(float d) {/*rot.y+=d; RotatedY = rot.y;*/
    }
    inline void YawLeft(float d) {/*rot.y-=d; RotatedY = rot.y;*/
    }
    
    void ViewFromCurrent() {
        f3 pos = this->GetPrs().GetPos();
        f3 rot = this->GetPrs().GetRot();
        glRotatef(rot.x, 1., 0., 0.);
        glRotatef(rot.y, 0., 1., 0.);
        glRotatef(rot.z, 0., 0., 1.);
        glTranslatef(-pos.x, -pos.y, -pos.z);
    }
    
	void OnPosRotChanged() {
        f3 pos = this->GetPrs().GetPos();
        f3 rot = this->GetPrs().GetPos();
		Position.x = pos.x;
		Position.y = pos.y;
		Position.z = pos.z;
		RotatedX = rot.x;
		RotatedY = rot.y;
		RotatedZ = rot.z;
	}

    void Update(float deltaT, GameObject* obj) {
		if (obj == this) {
			OnPosRotChanged();
		}

        if (mode==CAM_MODE_LOOKAT) {
           // aluLookAt(pos.x, pos.y, pos.z, obj->pos.x, obj->pos.y, obj->pos.z, 0.,1.,0.);
        }

		if (mode == CAM_MODE_2D) {
		//	aluLookAt(0., 0., 0., obj->pos.x, obj->pos.y, obj->pos.z, 0., 1., 0.);
		//	ViewFromCurrent();
		}
        
        if (mode==CAM_MODE_FPS || mode==CAM_MODE_FLY) {
            ViewFromCurrent();
        }
        
    }
    
    void NextMode() {
        mode++;
        if (mode>=CAM_MODE_LAST) {
            mode= CAM_MODE_NULL + 1;
        }
        if (mode==CAM_MODE_LOOKAT) printf("\tCameraMode:LOOKAT");
        if (mode==CAM_MODE_FPS) printf("\tCameraMode:FIRSTPERSON");
        if (mode==CAM_MODE_FLY) printf("\tCameraMode:FLY");
        if (mode==CAM_MODE_CHASE) printf("\tCameraMode:CHASE");
		if (mode == CAM_MODE_2D) printf("\tCameraMode:2D");
    }
    
	void processKeyDownWin(int key, float distStep) {
		if (mode == CAM_MODE_LOOKAT) {
			switch (key) {
			case KEY_FWD:
           	MoveAhead(distStep);
				break;
			case KEY_BAC:
				MoveBack(distStep);
				break;
			case KEY_LFT:
				MoveLeft(distStep);
				break;
			case KEY_RGT:
				MoveRight(distStep);
				break;
			case '3':
				MoveDown(distStep);
				break;
			case '9':
				MoveUp(distStep);
				break;
			default:
				break;
			}
		}
        if (mode == CAM_MODE_FPS) {
            switch (key) {
            case KEY_FWD:
                MoveAhead(distStep);
                break;
            case KEY_BAC:
                MoveBack(distStep);
                break;
            case KEY_LFT:
                MoveLeft(distStep);
                break;
            case KEY_RGT:
                MoveRight(distStep);
                break;
            case '3':
                MoveDown(distStep);
                break;
            case '9':
                MoveUp(distStep);
                break;
            default:
                break;
            }
        }
	}

    void processKeyDownMac(int key, float distStep) {
    
       if (mode==CAM_MODE_FPS) {
        switch (key) {
            case '8':
                MoveAhead(distStep);
                break;
            case '2':
                MoveBack(distStep);
                break;
            case '4':
                MoveLeft(distStep);
                break;
            case '6':
                MoveRight(distStep);
                break;
            case '3':
                MoveDown(distStep);
                break;
            case '9':
                MoveUp(distStep);
                break;
            default:
                break;
        }
       }
    }
};

#include "../XFunctions.hpp"

#define C_float float
#define Sin sin
#define Cos cos

char* New_String(const char* sz) {return ((char*)sz);}

void alPushI (int cmd, int x, int y);
void alPushP (int cmd, void* param1, void* param2);
int alLoadModel(ResourceInf* r);
void usingWithCaution_decrementModelCounter();
int alReserveModelId(void);
void alDrawModel (int ident, bool wireframe = false);

void alBillBoardBegin();
void alBillBoardEnd();
void alLoadIdentity();
void alPushMatrix();
void alPopMatrix();
void alaluLookAt(float x1, float y1, float z1, float x2, float y2, float z2, float x, float y, float z);
void alTriangle(float size);
void alLine(float x1, float y1, float x2, float y2);
void alCircle(float r, int segments);
void alRect(float w, float h);
void alLoadModel(char* alx, char* tga, int id, float size);
void alScaleModel(int id, float sx, float sy, float sz);
void alAlphaTest (int enable, float fA = 0.4); // 0=>unset ; 1=>set GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
float alModelBounds(int id);
void alDrawModelTranslateRotate (int id, float posx = 0.0, float posy = 0.0, float posz = 0.0, float angle = 0.0, float x = 0.0, float y = 0.0, float z = 0.0, int rotatefirst = 0, int billboard = 0);
void alTranslateRotate (float posx = 0.0, float posy = 0.0, float posz = 0.0, float angle =0.0, float x = 0.0, float y = 0.0, float z = 0.0);

#endif
