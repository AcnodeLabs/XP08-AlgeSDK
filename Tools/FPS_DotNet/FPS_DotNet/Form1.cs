using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Timers;

namespace FPS_DotNet
{
    public partial class Form1 : Form
    {
        Nats nats = new Nats();
        static Form1 me;
        private Thread natsThread;  // Store thread reference for cleanup

        public Form1()
        {
            InitializeComponent();
            nats.setCallback(onDataArrival);
            me = this;
            // Add FormClosing event handler
            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);
        }

        // Add FormClosing event handler
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Ensure clean shutdown
            if (natsThread != null && natsThread.IsAlive)
            {
                natsThread.Abort();  // Stop the NATS thread
            }
            Application.Exit();  // Exit the application
        }

        public List<string> toList(string data) {
            List<string> buttons = new List<string>();
            string[] names = data.Split('\n');
            Array.Resize(ref names, names.Length - 1);//lemove last element i.e }
            return names.ToList<string>();
        }

        public void onDataArrival(string data) {
            if (data.Contains("fps:"))
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    //listBox1.Items.Add(data);
                    string tofind = "fps:";
                    int igobs = data.IndexOf(tofind);
                    int ls = tofind.Length;
                    if (igobs >= 0) updateUIObjects(data.Substring(4));
                }));
            }
        }
        int interval = 5000;

        static Int64 old;

        string bytime(string value) {
            Int64 inp = Int64.Parse(value);
            Int64 diff = inp - old;
            old = inp;
            return value + "["+Convert.ToString(diff/interval)+"]";
        }

        private void updateUIObjects(string data)
        {
            label1.Text = data;//.Remove(data.Length-0);
            me.Width = label1.Width + 5;
        }

        private void NewButton_MouseClick(object sender, MouseEventArgs e)
        {
            char gobId = ((Button)sender).Text[0];
           // listBox1.Items.Add(((Button)sender).Text + "Was Clicked!!");
            PubIt("s"+ gobId);
        }

        public void WorkThreadFunction()
        {
            try
            {
                //-url nats://seaof-163-43-82-143.jp-tokyo-01.arukascloud.io:31759/ -subject AlgeApp.Evolution -verbose
                string[] args = {
                    "-url", Settings.server,//"nats://seaof-163-43-82-143.jp-tokyo-01.arukascloud.io:31759/",
                    "-subject", Settings.channel_reading(),
                    "-verbose"
                };
                nats.Run(args);
            }
            catch (Exception ex)
            {
                System.Console.Error.WriteLine("Exception: " + ex.Message);
                System.Console.Error.WriteLine(ex);
                return;
            }
        }

        // Update the Form_Load to store thread reference
        private void Form1_Load(object sender, EventArgs e)
        {
            natsThread = new Thread(new ThreadStart(WorkThreadFunction));
            natsThread.Start();
            string appName = Settings.appName();
            this.Show();
        }

        static void staticPubIt(string msg)
        {
            me.PubIt(msg);
        }

            void PubIt(string msg) {
            //"Usage:  Publish [-url url] [-subject subject] " +
            //     "-count [count] [-payload payload]");
            string[] args = {
                    "-url", FPS_DotNet.Settings.server,//"nats://seaof-163-43-82-143.jp-tokyo-01.arukascloud.io:31759/",
                    "-subject", FPS_DotNet.Settings.channel_writing(),
                     "-count", "1",
                    "-payload", msg
                };
            Publisher publisher = new Publisher();
            publisher.Run(args);
           
        }

        void wireframe(bool tf) {
          //  if (tf == true) PubIt("wt");
          //  if (tf == false) PubIt("wf");
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
           // wireframe(checkBox1.Checked);
        }

        private void Form1_Activated(object sender, EventArgs e)
        {
            System.Timers.Timer aTimer = new System.Timers.Timer();
            aTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            aTimer.Interval = interval;
            aTimer.Enabled = true;
        }

        // Specify what you want to happen when the Elapsed event is raised.
        private static void OnTimedEvent(object source, ElapsedEventArgs e)
        {
            //Console.Beep();
            staticPubIt("!rf");//report fps
        }

        private string LastChar(string s) {
          //  int len = cbTtype.Text.Length;
            string lastChar1 = s.Substring(s.Length - 1);
            return lastChar1;
        }

        private void btnTransform_Click(object sender, EventArgs e)
        {
           // PubIt("t"+LastChar(cbTtype.Text) + tbTval.Text);
        }
    }

}
