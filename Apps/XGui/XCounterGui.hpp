//
//  XGUI_App_Windows.cpp
//  AlgeApp
//
//  Created by xalmini on 31/08/2021.
//

using namespace ImGui;


class XCounterGui {
public:
    ImFont* counterFont;
    ImFont* defaultFont;

    XCounterGui() {
        for (int i=0;i<24;i++) {
         //   arr[i] = arr_max;
        }
    }

    ~XCounterGui() {
        SaveArr();
    }

    ImFont* AddDefaultFont(float pixel_size)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.SizePixels = pixel_size;
        config.OversampleH = config.OversampleV = 1;
        config.PixelSnapH = true;
        ImFont* font = io.Fonts->AddFontDefault(&config);
        return font;
    }
    string m_path;
    AlgeApp* m_app;

    void Init(string path, i2 appDim , AlgeApp* app) {
        appsize = appDim;
        m_path = path;
        m_app = app;
        defaultFont = AddDefaultFont(64);
        defaultFont->Scale = app->coord.getGuiScale();
        for_i(24)
            arr_sample[i] = (randm() * arr_max);
        _for

    }



    bool bR, bA, bL, bS, bC;

    void Center(int len) {
        float font_size = ImGui::GetFontSize() * len / 2;
        ImGui::SameLine(
                ImGui::GetWindowSize().x / 2 -
                font_size + (font_size / 2)
        );
    }

    bool Btn(string text, bool sameline = true) {
        ImVec2 spacerY(0.,50);
        ImVec2 spacerX(50.,0);
        bool btn = ImGui::Button(text.c_str());
        if (sameline) {
            SameLine();
         //   Dummy(spacerX);
        } else Dummy(spacerY);
        return btn;
    }
    char cTxt[8] = { 0 };

    short count;
    float arr[24];float arr_sample[24];

    int arr_max = 100;

    void Plt(float* data, int count) {
        i2 u = m_app->coord.getResolutionForGuiUsage();
        ImVec2 plotDimensions(0.9*u.x, u.y / 6.);
        ImGui::PlotHistogram("", data, count, 0, NULL, 0.0f, arr_max, plotDimensions);//hist has 1/5 of height of window
    }

    short m_min;
    short m_hr;
    short m_day;
    short m_month;

    void SaveArr() {
        FILE* f = fopen(getFname().c_str(), "w");
        if (f) {
            for (int i = 0; i < 24; i++) {
                int v = arr[i];
                fprintf(f, "%d,", v);
          //      arr[i] = 0;
            }
            fclose(f);
        }
    }
    int dailysum = 0;
    int LoadArr() {
        dailysum = 0;
        int curhourcount = 1;
        string fn = getFname();
        FILE* f = fopen(fn.c_str(), "r");
        if (f) {
            for (int i = 0; i < 24; i++) {
                int rr = 0;
                int nRead = fscanf(f, "%d,", &rr);
                arr[i] = rr / 1.0;
                if (i == m_hr) {
                    curhourcount = rr;
                }
                dailysum += rr;
                if (nRead <= 0) break;
            }
            fclose(f);
        }
        string am_pm = m_hr<13?"am":"pm";
        int mm_hr = m_hr;
        if (mm_hr >= 12) mm_hr -= 12;
        string am_pm2 = (m_hr+1)<13?"am":"pm";
        sprintf(mTitle, "Month%dDay%d %d%s-%d%s", m_month, m_day, mm_hr,am_pm.c_str(),mm_hr+1,am_pm2.c_str());
        return curhourcount;
    }

    string getFname() {
        static char szFname[256];
        sprintf(szFname, "%s/Month%dDay%d.txt", m_path.c_str(), m_month, m_day);
        return (string(szFname));
    }

    //day = 1 to 365;
    void onDayChanged(int daynow) {
        SaveArr();
        m_day = daynow;
    }

    char mTitle[64] = { '-' };
    void onAppOpen(int monthnow, int daynow, int hr_now) {
        m_day = daynow;
        m_month = monthnow;
        m_hr = hr_now;//Zero based
        m_month_onload = m_month;
        m_day_onload = m_day;
        count = LoadArr();
    }

    void onClose() {
        SaveArr();
    }

    void onHourChanged(int hournow) {
        count = 0;
        m_hr = hournow;
    }

    int m_month_onload = -1;
    int m_day_onload = -1;
    float fault = 0.0;
    float good = 0.0;

    i2 appsize;


    string getHelpText() {
        string help = "";
        help += "XCounter V1.1\n\n";
        help += "This is an Hourly counter App\n";
        help += "It Keeps hourly count records\n";
        help += "in 24 slots, one for each hour\n";
        help += "Press counter to increment\n";
        help += "Use <<< & >>> to navigate days\n";
        help += "Reset current hr if needed\n";
        help += "\nIt will help u track\n";
        help += "- total occurances\n";
        help += "- compare hourly trend\n";
        help += "- compare daily trend\n";
        help += "Example hourly histogram:\n";
        return help;
    }

    bool XBtn(const char* btnLabel, int modelId = 2) {
        bool f = Button(btnLabel);
        GameObject* btn_gob = 0;
        if (modelId >= 0) {
            for (int i = 0; i < m_app->nGobs; i++) {
                if (modelId == m_app->gobs[i]->modelId) {
                    btn_gob = m_app->gobs[i];
                    break;
                }
            }
            if (btn_gob) {
                int x = ImGui::GetItemRectSize().x;
                int y = ImGui::GetItemRectSize().y;
                int mx = ImGui::GetItemRectMin().x + x/2;
                int my = ImGui::GetItemRectMax().y - y/2;
                btn_gob->SetPos(mx , my);
            }
        }
        return f;
    }

    void ShowWindows(float dt, float scale) {

        sprintf(cTxt, "%02d", count);
        ImVec2 spacer(0.,50);
        PushFont(defaultFont);
        PushStyleColor(ImGuiCol_WindowBg, ImVec4(fault, good, 0, 1.f));
        fault -= dt;
        if (fault < 0.5) fault = 0;
        good -= dt;
        if (good < 0.5) good = 0;

        static bool show_help = false;

        if (show_help) {
            SetNextWindowPos(spacer);
            ImGui::Begin("Help", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_Modal);
            PushFont(defaultFont);
            PushStyleColor(ImGuiCol_WindowBg, ImVec4(fault, good, 0, 1.f));
            Text("%s", getHelpText().c_str());
            Plt(arr_sample, IM_ARRAYSIZE(arr_sample));
            if (Button("\n Close \n ")) {
                show_help = false;
            }
            ImGui::PopStyleColor();
            PopFont();
            ImGui::End();
        }


      //  Dummy(spacer);
        ImGui::Begin(mTitle,0 ,ImGuiWindowFlags_NoBackground);
      //  Dummy(spacer);


        SameLine();
        if (Button("\n Help \n______\n")) {
            show_help = true;
        }


        SameLine();
        if (XBtn("\n Save \n______\n")) {
            good = 1.0;
            SaveArr();
        }


            if (Btn("      \n  <<<<  \n        \n")) {
                SaveArr();
                int tm = m_month;
                int td = m_day;
                m_day--;
                if (m_day < 0) {
                    m_month--; m_day = 31;
                }
                FILE* f = fopen(getFname().c_str(), "r");
                if (f)  {
                    fclose(f);
                    count = LoadArr();
                    good = 1.;
                }
                else {
                    fault = 1.;
                    m_month = tm;
                    m_day = td;
                }
            };

            if (XBtn("        \n  >>>>  \n        \n")) {
                SaveArr();
                int tm = m_month;
                int td = m_day;
                m_day++;
                if (m_day > 31) { m_month++; m_day = 1; }
                FILE* f = fopen(getFname().c_str(), "r");
                if (f) {
                    fclose(f);
                    count = LoadArr();
                    good = 1.;
                }
                else {
                    fault = 1.;
                    m_month = tm;
                    m_day = td;
                }
            };

            Plt(arr, IM_ARRAYSIZE(arr));
            Button(("Daily Sum = " + to_string(dailysum)).c_str());
        
            SetWindowFontScale(scale*5);

            if (Btn(cTxt,false)) {
                count++;
                dailysum++;
                arr[m_hr] = count;
            }

            SetWindowFontScale(1);

            Dummy(half(spacer));

        //SameLine();
        //Center(10);
        if (XBtn("\n Reset Hr \n__________\n")) {
            count = 0;
            arr[m_hr] =0;
        }

        ImGui::PopStyleColor();
        PopFont();
        ImGui::End();

    }

    ImVec2 half(ImVec2 v) {
        return ImVec2(v.x/2,v.y/2);
    }
};
