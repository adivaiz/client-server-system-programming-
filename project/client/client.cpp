// Authors:  Meir Haimov, Adi Vaizman.
// ID's: 207910787, 212555726.
#include <arpa/inet.h> // כולל את הפונקציות הנדרשות לניתוב אינטרנט
#include <cstdlib> // כולל את הפונקציה atoi להמרת מחרוזת למספר שלם
#include <iostream> // כולל את הפלט והקלט הסטנדרטי
#include <netinet/in.h> // כולל את מבנה הנתונים sockaddr_in, המשמש כדי לתאר כתובת אינטרנט
#include <string> // כולל את המחלקה מחרוזת ופונקציות קשורות
#include <sys/socket.h> // כולל את הפונקציות הנדרשות לניהול קצה-שקצה של חיבורי רשת
#include <unistd.h> // כולל את הפונקציות הנדרשות לשימוש במערכת הקבצים והסביבה של המערכת
#include <cstring> // כולל פונקציות לניהול רצפי תווים

using namespace std; // מאפשר שימוש באובייקטים ופונקציות מספרי סטנדרט של שפת C++

const int SIZE = 1024; //   הגדרת קבוע לגודל המערך שמקבל את הנתונים מהשרת
//אתחול החיבור
//מקום האפס זה a.out ומקום אחד כתובת איי פי 
int main(int argc, char **argv) { // הפונקציה הראשית
    int port = atoi(argv[2]); // המרת הפורט למספר שלם
    int start = atoi(argv[3]); // המרת נקודת ההתחלה בגרף למספר שלם
    int fd = socket(AF_INET, SOCK_STREAM, 0); // יצירת סוקט חדש
    sockaddr_in server_addr = {0}; // הגדרת כתובת לשרת
    memset(&server_addr, 0, sizeof(server_addr)); // הפונקציה משמת לאתחול בלוק זיכרון כאשר המשתנה הראשון הוא מצביע לתחילת הזיכרון השני זה הערך שממלאים והשלישי זה כמות בתים
    server_addr.sin_family = AF_INET; // הגדרת סוג הכתובת
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // הגדרת כתובת השרת
    server_addr.sin_port = htons(port); // הגדרת הפורט
// חיבור ושליחת נתונים
    connect(fd, (sockaddr *)&server_addr, sizeof(server_addr)); // חיבור לשרת
    cout << "Sending data to server: " << argv[3] << " " << argv[4] << endl; // הדפסת הנתונים שמתקבלים מהקליינט
    string send = string(argv[3]) + " " + string(argv[4]); // יצירת מחרוזת המכילה את נקודת ההתחלה והיעד
    // כתיבה לשרת של סנד ואם הגודל של הבתים שכתבנו שונה מאורך המחרוזת תחזיר שגיאה
    if (send.size() != write(fd, send.c_str(), send.size())) { // בדיקה שכל הנתונים נשלחו בהצלחה
        cerr << "Error: Failed to send all data to server\n"; // הדפסת הודעת שגיאה
        close(fd); // סגירת חיבור
        return 1; // החזרת ערך היציאה
    }
//קבלת הנתונים
    char buffer[SIZE]; // הגדרת מערך לקבלת הנתונים מהשרת
    int howmuch = recv(fd, buffer, SIZE - 1, 0); // קבלת הנתונים מהשרת
    if (howmuch > 0) { // בדיקה שהנתונים התקבלו בהצלחה
        buffer[howmuch] = '\0'; // הוספת סיומת למחרוזת
        cout << " The Shortest path is : " << buffer << endl; // הדפסת המסלול הקצר ביותר
    }
    close(fd); // סגירת חיבור

    return 0; // החזרת ערך היציאה
}
