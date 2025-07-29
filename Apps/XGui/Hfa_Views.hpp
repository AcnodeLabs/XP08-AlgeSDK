
void(*currentView)(void);
static IceTop ice;

class IceLog {
    
public:
    static void LogIt(string message) {
        ice.proj = "HfaApp";
        ice.Put("log", message);
    }
};

class Authenticator {
public :

    static bool doLogin(string username, string password) {
        ice.proj = "XalAuth";

        string mycred = ice.Get("Login/" + username);
        bool ok = (mycred == password);
        if (ok) IceLog::LogIt("user " + username + " logged in");
        return ok;
    }

    static bool signUp(string username, string password)
    {
        ice.proj = "XalAuth";
        bool mycred = ice.Put("Login/" + username, password);
        return doLogin(username, password);
    }

};


char username[64];
char password[64];

void PrimaryScreen();
void LoginPage();
void MainPage();
void ItemsDetailPage();


bool doLogin() {
    bool ok = Authenticator::doLogin(string(username), string(password));
    if (ok) currentView = PrimaryScreen;
    return ok;
}
bool doSignUp() {
    ice.proj = "XalAuth";
    bool mycred = ice.Put("Login/" + string(username), string(password));
    return doLogin();
}

void MainPage() {
    ImGui::ShowDemoWindow();
}

void Footer() {
    Spacing();
    Text("Powered by XAL");
}

void btnSaveItem() {}
void btnEditPicture() {}
void ItemsDetailPage() {
    using namespace ImGui;
    {
        Begin("ItemsDetailScreen");
        if (Button("Save / Update")) btnSaveItem();
        Text("Daal Mash"); Text("Image"); SameLine(); if (Button("Change Pic")) btnEditPicture();
        int spice = 5;
        SliderInt("Spice Level", &spice, 0, 10);
        char preptime[32] = { 0 };
        char servings[32] = { 0 };
        char instructions[128] = { 0 };
        char ingredients[256] = { 0 };
        InputText("Preparation Time", preptime, 32 );
        InputText("Servings:", servings, 32);
        InputText("Pickup Instructions:", instructions, 128);
        Spacing();
        InputText("Ingredients:", ingredients, 256);
        if (Button("<<<Back")) currentView = PrimaryScreen;
        Footer();
        End();
    }
}

void LoginPage() {
    using namespace ImGui;
    {
        Begin("LoginPage");
        Text("Welcome to XAL Network");
        Spacing();
        Text("Sign in with your existing credentials of make a new accont");
        InputText("Enter Username", username, 64);
        InputText("Enter Password", password, 64);
        if (Button("Login")) doLogin();
        if (Button("SignUp")) doSignUp();
        Footer();
        End();
    }
}

void btnOrder() {}
void btnManage() { currentView = ItemsDetailPage; }

void PrimaryScreen() {
    using namespace ImGui;
    {
        Begin("Home Food Around");
        Text("Welcome to HomeFoodAround Network");
        Spacing();
        Text("IMAGE"); SameLine(); if (Button("Order Some Food")) btnOrder();
        Text("IMAGE"); SameLine(); if (Button("Manage Your Kitchen")) btnManage();
        Footer();
        End();
    }
}