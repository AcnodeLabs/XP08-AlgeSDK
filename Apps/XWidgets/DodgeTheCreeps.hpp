// Copyright (c) 2018 AcnodLabs Inc
//Clone of GoDot's Example found at https://docs.godotengine.org/en/3.1/getting_started/step_by_step/your_first_game.html
//////////////////////////////XTimer
// on device : respath = /data/user/0/com.acnodelabs.DodgeTheCreeps/files

using namespace ImGui;

///TODO
///Flicker, Trail
class /*DodgeTheCreeps*/ App : public AlgeApp { 

	GameObject background, txt;
	XWalkingSprite playerGrey;
	GameObject playerTrail;
	XFlipFlopSprite enemyFlyingAlt[3], enemySwimming[3], enemyWalking[3];
	GameObject count_text;
//	XmlReader xamlMainWindow, xamlHelpWindow;
	
public:

    void avoidOrigin() {
        for_i(x3)
            enemyFlyingAlt[i].avoidOrigin();
            enemySwimming[i].avoidOrigin();
            enemyWalking[i].avoidOrigin();
        _for
    }
    
	void MakeTrail() {
		playerTrail.prsInstances.clear();
		//return;
		PosRotScale bp;
		bp.CopyFrom((PosRotScale*)&playerGrey);
		
		for (int i = 0; i < 4; i++) {
			bp.scale = 1.0;
			playerTrail.AddInstance(bp);
		}
	}

	virtual void Init(char* path) {
        x3 = 3;
        quiet = true;
		AlInit(STANDARD);

		SetTitle("DodgeTheCreeps");
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
		AddResource(&background, "bg_u", "bg_u.jpg", XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
		
		//AddResource(&playerTrail, "playerGrey_up1", 1);
		//MakeTrail();

		playerGrey.LoadIn(this, "playerGrey_up", "playerGrey_walk");

		for_i(x3)
			enemyFlyingAlt[i].LoadIn(this, "enemyFlyingAlt_");
			enemySwimming[i].LoadIn(this, "enemySwimming_");
			enemyWalking[i].LoadIn(this, "enemyWalking_");
		_for

		with AddObject(&count_text);
			_.scale = 2.0;
			_.pos.x = rightSide / 2;
			_.pos.y = bottomSide * 0.1;
		_with

		output.pushP(CMD_SNDSET0, $ "house.wav");
		output.pushP(CMD_SNDSET1, $ "gameover.wav");
		output.pushP(CMD_SNDPLAY0, $ "house.wav");
        
        AddResource(&txt, "text");

        imHelper.SetFont(path, "Roboto-Bold.ttf", getPreferredTextSize());
	//	guiFont.Init(string(path)+"/Roboto-Bold.ttf", 22.0f);
        		//
	//	xamlMainWindow.Init(path)->Load("Views/XAML/MainWindow.xaml");
	//	xamlHelpWindow.Init(path)->Load("Views/XAML/HelpWindow.xaml");
	}

	CControls c;
    bool quiet;



    
	void processInput(PEG::CMD* p, float deltaT) {

		if (p->command == CMD_SCREENSIZE) {
			c.screen[0] = p->i1;
			c.screen[1] = p->i2;
		}

		if (p->command == CMD_KEYDOWN) {

			if (p->i1 == AL_KEY_SPACE) {
 				centerPlayer();
			}

            if (p->i1 == 'w' || p->i1 == 'W') {
                wireframe = !wireframe;
            }

			if (p->i1 == AL_KEY_PLUS) {
			}
			if (p->i1 == AL_KEY_MINUS) {
			}

          
            if (p->i1 == 'q' ||  p->i1 == 'Q') {
                quiet = !quiet;
	//			xamlMainWindow.checkboxes[0] = quiet;
            }
            
			if (p->i1 == KEY_RGT) {
				playerGrey.SetIntent('R');
			}
			if (p->i1 == KEY_LFT) {
				playerGrey.SetIntent('L');
			}
			if (p->i1 == KEY_FWD) {
				playerGrey.SetIntent('U');
			}
			if (p->i1 == KEY_BAC) {
				playerGrey.SetIntent('D');
			}
		}
        
		if (p->command == CMD_TOUCH_START) {
			short k = c.KROSS_(false, p->i1, p->i2);
			char t = c.toChar(k);
			if (t == 'C') t = ' ';
			playerGrey.SetIntent(c.toChar(c.KROSS_(false, p->i1, p->i2)));
		}

		if (p->command == CMD_TOUCH_END) {
	//		quiet = xamlMainWindow.checkboxes[0];
		//	x3 = xamlHelpWindow.isliders[0];
		}

	}

	
	//https://docs.google.com/spreadsheets/d/1_TFyagNzOVhD4MxVY3VLq7RzB8og0wDuWRb5HTW-KFg/edit#gid=0
	void trailLogic(float deltaT) {
		static int i = 0;
		static float trailVar = 0.0f;
		trailVar += deltaT;
		f3 p = playerGrey.GetPos();
		int w = playerGrey.GetWidth();

		for (int i = 0; i < int(playerTrail.prsInstances.size());i++) {
			float inv = 0.9 / sqrt(i+1);
			with (playerTrail.getInstancePtr(i));
				_.scale = sqrt(inv);
				p.x += (w /(i+1) );
				_.pos = p;
			_with
	 	}
	}

	void centerPlayer() {
		playerGrey.SetPos(f3(rightSide / 2., bottomSide / 2., 0.));
	}
	
	void intersectLogic(GameObject* gob) {
		
		if (doObjectsIntersect((PosRotScale*)& (playerGrey.playerGrey_up1),gob) ){
			timeVar = 0;
			if (!quiet) output.pushP(CMD_SNDPLAY1, $ "gameover.wav");
			centerPlayer();
			playerGrey.WasHit();
            avoidOrigin();
		}
	}
    bool wind;
    int x3;
    
	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}


    void RenderGui(float deltaT) {
		InfGuiHover(); return;
        if (!txt.hidden) {
            ImGui::SetNextWindowBgAlpha(0.5);
            ImGui::Begin("Score", &wind, ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("Score:%s", to_string(int(timeVar)).c_str());
            ImGui::Checkbox("Quiet", &quiet);
            ImGui::SliderInt("x3", &x3, 1, 3);
            ImGui::End();

           /* {
                static int i1 = 50, i2 = 42;
                ImGui::DragInt("drag int", &i1, 1);
                ImGui::SameLine();
                HelpMarker(
                        "Click and drag to edit value.\n"
                        "Hold SHIFT/ALT for faster/slower edit.\n"
                        "Double-click or CTRL+click to input value.");

                ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%",
                               ImGuiSliderFlags_AlwaysClamp);

                static float f1 = 1.00f, f2 = 0.0067f;
                ImGui::DragFloat("drag float", &f1, 0.005f);
                ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
            } */

            //			xamlMainWindow.Render();
            //			xamlHelpWindow.Render();

			imHelper.ShowFPS(getBackgroundSize(), "fps:", deltaT, f3(0.,0.,0.));
        }
    }

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
		
        if (gob->is(txt)) {

        }
        
		if (gob->is(playerTrail)) {
			trailLogic(deltaT);
		}

		if (playerGrey.IsItYou(gob)) {
			playerGrey.Update(deltaT, this);
		}

        for_i(3)
          //  enemyFlyingAlt[i].hide(true);
          //  enemySwimming[i].hide(true);
          //  enemyWalking[i].hide(true);
        _for
        
		for_i(x3)
        
//            enemyFlyingAlt[i].hide(false);
//            enemySwimming[i].hide(false);
//            enemyWalking[i].hide(false);
        
			if (enemyFlyingAlt[i].IsItYou(gob)) {
				enemyFlyingAlt[i].Update(deltaT, this);
				intersectLogic(gob);
			}

			if (enemySwimming[i].IsItYou(gob)) {
				enemySwimming[i].Update(deltaT, this);
				intersectLogic(gob);
			}

			if (enemyWalking[i].IsItYou(gob)) {
				enemyWalking[i].Update(deltaT, this);
				intersectLogic(gob);
			}
		_for
	}

};

