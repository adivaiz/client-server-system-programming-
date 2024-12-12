// Authors:  Meir Haimov, Adi Vaizman.
// ID's: 207910787, 212555726
#include <arpa/inet.h> // כולל את הפונקציות הנדרשות לניתוב אינטרנט
#include <iostream> // כולל את הפלט והקלט הסטנדרטי
#include <map> // כולל את מבנה הנתונים מפה, שבו מתאים בין מפתח וערך
#include <netinet/in.h> // כולל את מבנה הנתונים sockaddr_in, המשמש כדי לתאר כתובת אינטרנט
#include <thread> // כולל את היצירה וניהול של תהליכים קטנים
#include <string> // כולל את המחלקה מחרוזת ופונקציות קשורות
#include <sys/socket.h> // כולל את הפונקציות הנדרשות לניהול קצה-שקצה של חיבורי רשת
#include <sys/types.h> // כולל את מבני הנתונים המתחילים בטיפוסים הבסיסיים לכל המערכת
#include <unistd.h> // כולל את הפונקציות הנדרשות לשימוש במערכת הקבצים והסביבה של המערכת
#include <vector> // כולל את מבנה הנתונים מערך דינמי
#include <mutex> // כולל את הפונקציות הנדרשות לנעילת תחום בזמן שימוש במשאב משותף
#include <queue> // כולל את מבנה הנתונים תור
#include <sstream> // כולל פונקציות לביצוע פעולות פרסום והמרה בין סוגי נתונים
#include <fstream> // כולל פונקציות קריאה וכתיבה לקבצים
#include <cstring> // כולל פונקציות לניהול רצפי תווים
using namespace std; // מאפשר שימוש באובייקטים ופונקציות מספרי סטנדרט של שפת C++

int const MAX_SIZE = 1024; // הגדרת קבוע שמגדיר את הגודל המרבי של המערך בתווים

vector<int> shortestPath(vector<vector<int>> &graph, int start, int end) { // שולחים & ביי רפרנס כי שכפול של וקטור יקר מבחינת מקום וזמן אז עדיף לשלוח את המקור
    // פונקציה שמחזירה את המסלול הקצר ביותר בגרף מצומת התחלתי לצומת סופי 
    vector<int> path; 
    // רשימת נתיבים שתאחסן את המסלול הקצר ביותר 
    if (start < 0 || start >= graph.size() || end < 0 || end >= graph.size()) { 
        // בדיקה אם הצמתים ההתחלתי והסופי הם ערכים חוקיים בגבולות הגרף 
        cerr << "Error: Invalid start or end node, check your numbers and try again.\n"; 
        // אם הצמתים לא חוקיים, מדפיסים הודעת שגיאה
        return path; 
        // מחזירים מסלול ריק
    }
    vector<bool> ifvisited(graph.size(), false); 
    // יצירת וקטור בוליאני בגודל הגרף, כאשר כל האיברים מאופסים (false)
    // הווקטור הזה ישמש למעקב אחר צמתים שביקרנו בהם
    vector<vector<int> > paths(graph.size()); 
    // יצירת וקטור של וקטורים לאחסון המסלולים לצמתים השונים
    // כל איבר ב-`paths` יכיל את המסלול לצומת המתאים באינדקס
    queue<int> q; 
    // יצירת תור חדש שינוהל לפי שיטת FIFO (First In, First Out) 
    q.push(start); 
    // דוחפים את הצומת ההתחלתי לתור
    ifvisited[start] = true; 
    // מסמנים את הצומת ההתחלתי כצומת שביקרנו בו
    while (!q.empty()) { 
        // כל עוד התור אינו ריק  
        int node = q.front(); 
        // מקבלים את הצומת הראשון בתור (ללא הסרה)
        q.pop(); 
        // מסירים את הצומת הראשון מהתור
        if (node < 0 || node >= graph.size()) { 
            // בדיקה אם הצומת הנוכחי חוקי (בתוך גבולות הגרף)
            cerr << "Error: Invalid node in graph, try another numbers\n"; 
            // אם הצומת אינו חוקי, מדפיסים הודעת שגיאה
            continue; 
            // ממשיכים לאיטרציה הבאה בלולאה
        }   
        for (int neighbor : graph[node]) { 
            // לולאה שעוברת על כל השכנים של הצומת הנוכחי    
            if (!ifvisited[neighbor]) { 
                // אם לא ביקרנו בצומת השכן 
                ifvisited[neighbor] = true; 
                // מסמנים את השכן כצומת שביקרנו בו  
                paths[neighbor] = paths[node]; 
                // מעתיקים את המסלול לצומת השכן מהמסלול לצומת הנוכחי    
                paths[neighbor].push_back(neighbor); 
                // מוסיפים את הצומת השכן למסלול               
                q.push(neighbor); 
                // דוחפים את הצומת השכן לתור
            }
        }
    }
    return paths[end]; 
    // מחזירים את המסלול לצומת הסופי
}


vector<vector<int>> cgraph(const string& filename) { // פונקציה שמקבלת שם קובץ ומחזירה גרף
    vector<vector<int>> graph; //  ומייצג שכנויות מערך של מערכים המייצג את הגרף
    ifstream file(filename); // פתיחת קובץ לקריאה
    if (file.is_open()) { // בדיקה אם נפתח הקובץ בהצלחה
        string line; // מחרוזת לקריאת השורה הנוכחית בקובץ
        while (getline(file, line)) { // קריאת השורה הנוכחית בקובץ
            stringstream ss(line); // המרת השורה לסטרינג סטרימ
            long long int node1, node2; // המשתנים לקריאת צמתים מהשורה
            if (ss >> node1 >> node2) { //מפרקים את השורה לשני צמתים ומכיניסים לחוליה
                while (graph.size() <= max(node1, node2)) { // להגדיל את הגרף בשביל שיכיל את כל הצמתים שנמצאים בקובץ
                    graph.push_back({}); // הוספת רשימה ריקה למערך המקבץ
                }
                graph[node1].push_back(node2); // הוספת הצומת השני לרשימת השכנים של הצומת הראשון
                graph[node2].push_back(node1); // הוספת הצומת הראשון לרשימת השכנים של הצומת השני
            } else {
                cerr << "Invalid line in graph file, please insert different numbers and try argain! \n"; // הדפסת הודעת שגיאה במקרה שהשורה אינה חוקית
            }
        }
        file.close(); // סגירת קובץ
    } else {
        cerr << "Unable to open graph file, please try again!\n"; // הדפסת הודעת שגיאה במקרה שהקובץ לא נפתח
    }
    return graph; // החזרת הגרף
}

mutex m; // הגדרת אובייקט מנעילת תחום לשימוש משותף
map<pair<int, int>, vector<int> > tenLast; // הגדרת מפה שמקשרת בין זוגות של מספרים לבין מסלולים

void receiveClient(int fd, vector<vector<int> > &graph) { // פונקציה לקבלת קליינט
    char buffer[MAX_SIZE]; // הגדרת מערך לקבלת הנתונים מהקליינט
    int reader = recv(fd, buffer, MAX_SIZE, 0); // קבלת הנתונים מהקליינט
    if (reader > 0) { // בדיקה שהנתונים התקבלו בהצלחה
        buffer[reader] = '\0'; // לציין שהמחרוזת מסתיימת 
        int start = -1, end = -1;  // הגדרת משתנים להתחלה וסיום המסלול
        stringstream ss(buffer); // המרת הנתונים לסטרינג סטרימ
        ss >> start >> end; // קריאת הנתונים מהסטרינג סטרימ
        vector<int> result; // הגדרת משתנה לשמירת התוצאה
        { //  כדי לשלוט בעומס התחלת בלוק מנעילת תחום
            unique_lock<mutex> lock(m); // הגדרת אובייקט לנעילת תחום
            auto it = tenLast.find({start, end}); //  מחפש האם בעשיריה האחרונה שחיפשנו קיים הזוג הנל אם לא קיים מחזיר מצביע לסוף
            if (it != tenLast.end()) { // אם החזיר איטרטור כלומר קיים
                result = it->second; //  את האורך בין הנקודות לריזולט הכנס את המסלול
            }
        } // סיום בלוק מנעילת תחום
        // אם לא נמצא בעשר האחרונים חפש את המסלול הקצר
        vector<int> path; // הגדרת משתנה לשמירת המסלול
        if (result.empty()) { // בדיקה אם התוצאה ריקה
            path = shortestPath(graph, start, end); // מציאת המסלול הקצר ביותר
            { // התחלת בלוק מנעילת תחום
                unique_lock<mutex> lock(m); //למצב שהרבה מתחברים יחד אז נועלים משנים ומשחררים
                if (tenLast.size() >= 10) { // בדיקה אם יש יותר מעשרה מסלולים
                    tenLast.erase(tenLast.begin()); // מחיקת המסלול הראשון
                }
                tenLast[{start, end}] = path; // הוספת המסלול למפה
            } // סיום בלוק מנעילת תחום
        } else { // אחרת
            path = result; // השמה של התוצאה למסלול
        }
        stringstream response; // אובייקט שנותן תשובה ללקוח
        response << start << " "; // שרשור נקודת ההתחלה
        for (int node : path) { // קריאה מהוקטור של המסלול
            response << node << " "; //הרספונס משרשר ונוד זה המסלול בהדפסה 
        }
        response << "\n"; // סיום קו בתשובה
        string answer = response.str(); // המרת התשובה למחרוזת
        write(fd, answer.c_str(), answer.size()); // שליחת התשובה לקליינט
    }
    close(fd); // סגירת חיבור
}

int main(int argc, char** argv) { // הפונקציה הראשית
    vector<vector<int>> graph = cgraph(argv[1]); // קריאת הגרף מהקובץ
    int port = atoi(argv[2]); // קריאת הפורט מהארגומנטים
    int fd = socket(AF_INET, SOCK_STREAM, 0); // יצירת סוקט חדש
    sockaddr_in server_addr = {0}; // הגדרת כתובת לסרבר
    memset(&server_addr, 0, sizeof(server_addr)); // מילוי הכתובת באפסים
    server_addr.sin_family = AF_INET; // הגדרת סוג הכתובת
    server_addr.sin_addr.s_addr = INADDR_ANY; // הגדרת הכתובת לכל הכתובות האפשריות
    server_addr.sin_port = htons(port); // הגדרת הפורט
    bind(fd, (sockaddr*)&server_addr, sizeof(server_addr)); // קשירת הסוקט לפורט
    listen(fd, 2); // ולכמה מאפשרת לחכות בתור האזנה לסוקט
    while (true) { // לולאת אינסוף
        sockaddr_in clientAddr; // הגדרת כתובת לקליינט
        socklen_t clientSize = sizeof(clientAddr); // הגדרת גודל הכתובת
        int clientSocket = accept(fd, (sockaddr*)&clientAddr, &clientSize); // מחזיר מספר סוקט שני כאשר הפונקציה יוצרת סוקטלהעברת מידע והישן להתחברות
        thread clientThread(receiveClient, clientSocket, ref(graph)); // יצירת תהליך חדש לטיפול בקליינט
        clientThread.detach(); // התנתקות מהחוט הנוכחי
    }
    close(fd); // סגירת הסוקט
}

