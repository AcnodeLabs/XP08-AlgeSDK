using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace FPS_DotNet
{
    internal class Settings
    {
        public static string server = "nats://nats-server:4222";
        public static string channel_reading() { return "xal." + appName() + ".out"; }
        public static string channel_writing() { return "xal." + appName();}

        static string m_appName = ""; // Default value, can be changed dynamically if needed

        public static string appName() {
            if (m_appName == "") {
                string toolspath = ExecutablePath.Substring(0, ExecutablePath.IndexOf("\\Tools") + "\\Tools".Length); ;
                string candidate = toolspath + "\\..\\Platforms\\CANDIDATE.h";

                try
                {
                    if (System.IO.File.Exists(candidate)) {
                        string[] lines = System.IO.File.ReadAllLines(candidate);
                        foreach (string line in lines) {
                            if (line.Contains("#include ") && line.Contains(".hpp")) {
                                // Extract the app name from the path
                                string fullPath = line.Split('/')[2].Trim('"');
                                m_appName = System.IO.Path.GetFileNameWithoutExtension(fullPath);
                                break;
                            }
                        }
                    }
                } catch (Exception ex) {
                    Console.WriteLine("Should be Run From AlgeSDK/Tools directory only: " + ex.Message);
                }
            }
            return m_appName; // TODO: Implement logic to find the app name dynamically if needed  
        }

        // Get the directory where the executable is running
        public static string ExecutablePath
        {
            get
            {
                return Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            }
        }
    }
}
