#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Blood packet class
class BloodPacket {
private:
    string type;       // Blood type (A, B, AB, O)
    string rhesus;     // Rhesus factor (+ or -)
    time_t collectDate;
    time_t expiryDate; // Blood has shelf life of ~42 days
    string donorId;    // Reference to the donor
    bool allocated;    // Whether it's allocated to a hospital

public:
    BloodPacket(string t, string rh, string donor) : 
        type(t), rhesus(rh), donorId(donor), allocated(false) {
        collectDate = time(nullptr);
        // Set expiry date to 42 days from now
        expiryDate = collectDate + (42 * 24 * 60 * 60);
    }

    string getType() const { return type; }
    string getRhesus() const { return rhesus; }
    string getFullType() const { return type + rhesus; }
    string getDonorId() const { return donorId; }
    bool isAllocated() const { return allocated; }
    
    // Check if blood packet is expired
    bool isExpired() const {
        return time(nullptr) > expiryDate;
    }
    
    void allocate() { allocated = true; }
    void deallocate() { allocated = false; }
    
    void displayInfo() const {
        cout << "Blood Type: " << type << rhesus << endl;
        
        // Convert time_t to human-readable format
        char collectionDate[26];
        char expiryDateStr[26];
        ctime_s(collectionDate, sizeof(collectionDate), &collectDate);
        ctime_s(expiryDateStr, sizeof(expiryDateStr), &expiryDate);
        
        cout << "Collection Date: " << collectionDate;
        cout << "Expiry Date: " << expiryDateStr;
        cout << "Donor ID: " << donorId << endl;
        cout << "Status: " << (allocated ? "Allocated" : "Available") << endl;
    }
};

class Donor {
private:
    static int nextId;
    string id;
    string name;
    string contact;
    string bloodType;
    string rhesus;
    bool fitnesStatus;
    time_t lastDonation;

public:
    Donor() : fitnesStatus(false), lastDonation(0) {
        id = "D" + to_string(nextId++);
    }
    
    void registerDonor() {
        cout << "\n===== DONOR REGISTRATION =====" << endl;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter Contact Number: ";
        getline(cin, contact);
        
        while (true) {
            cout << "Enter Blood Type (A, B, AB, O): ";
            cin >> bloodType;
            
            // Convert to uppercase
            for (char &c : bloodType) {
                c = toupper(c);
            }
            
            if (bloodType == "A" || bloodType == "B" || bloodType == "AB" || bloodType == "O") {
                break;
            } else {
                cout << "Invalid blood type. Please try again." << endl;
            }
        }
        
        // Rhesus factor input validation
        while (true) {
            cout << "Enter Rhesus Factor (+ or -): ";
            cin >> rhesus;
            
            if (rhesus == "+" || rhesus == "-") {
                break;
            } else {
                cout << "Invalid rhesus factor. Please enter + or -." << endl;
            }
        }
        
        int fitnessInput;
        cout << "Enter Fitness Status (1-Fit, 0-Unfit): ";
        cin >> fitnessInput;
        fitnesStatus = (fitnessInput == 1);
        
        cout << "\nDonor Registration Successful!" << endl;
        cout << "Your Donor ID is: " << id << endl;
    }
    
    // Check eligibility for donation
    bool isEligibleToDonate() const {
        // Check fitness
        if (!fitnesStatus) {
            return false;
        }
        
        // Check if enough time has passed since last donation (56 days)
        if (lastDonation != 0) {
            time_t now = time(nullptr);
            double seconds = difftime(now, lastDonation);
            double days = seconds / (60 * 60 * 24);
            
            if (days < 56) {
                return false;
            }
        }
        
        return true;
    }
    
    // Record a donation
    void recordDonation() {
        lastDonation = time(nullptr);
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getBloodType() const { return bloodType; }
    string getRhesus() const { return rhesus; }
    string getFullBloodType() const { return bloodType + rhesus; }
    
    // Display donor information
    void displayInfo() const {
        cout << "Donor ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Contact: " << contact << endl;
        cout << "Blood Type: " << bloodType << rhesus << endl;
        cout << "Fitness Status: " << (fitnesStatus ? "Fit" : "Unfit") << endl;
        
        if (lastDonation != 0) {
            char dateStr[26];
            ctime_s(dateStr, sizeof(dateStr), &lastDonation);
            cout << "Last Donation: " << dateStr;
            
            time_t nextEligible = lastDonation + (56 * 24 * 60 * 60);
            ctime_s(dateStr, sizeof(dateStr), &nextEligible);
            cout << "Next Eligible Donation: " << dateStr;
        } else {
            cout << "Last Donation: Never" << endl;
        }
    }
};
int Donor::nextId = 1000;

class Hospital {
private:
    static int nextId;
    string id;
    string name;
    string contact;
    bool paymentStatus;

public:
    Hospital() : paymentStatus(false) {
        id = "H" + to_string(nextId++);
    }
    
    // Register a new hospital
    void registerHospital() {
        cout << "\n===== HOSPITAL REGISTRATION =====" << endl;
        cout << "Enter Hospital Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter Contact Number: ";
        getline(cin, contact);
        
        cout << "\nHospital Registration Successful!" << endl;
        cout << "Your Hospital ID is: " << id << endl;
    }
    
    // Check payment status
    void checkPayment() {
        char choice;
        cout << "Has payment been made? (Y/N): ";
        cin >> choice;
        
        if (toupper(choice) == 'Y') {
            paymentStatus = true;
            cout << "Transaction complete!" << endl;
        } else {
            paymentStatus = false;
            cout << "Transaction incomplete!" << endl;
        }
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    bool hasPayment() const { return paymentStatus; }
    
    // Display hospital info
    void displayInfo() const {
        cout << "Hospital ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Contact: " << contact << endl;
        cout << "Payment Status: " << (paymentStatus ? "Paid" : "Pending") << endl;
    }
};
int Hospital::nextId = 1000;

class BloodBank {
private:
    string username;
    string password;
    vector<Donor> donors;
    vector<Hospital> hospitals;
    vector<BloodPacket> inventory;
    
    // Blood inventory counts
    struct BloodCount {
        int aPos = 0, aNeg = 0;
        int bPos = 0, bNeg = 0;
        int abPos = 0, abNeg = 0;
        int oPos = 0, oNeg = 0;
    } bloodCount;
    

public:
    BloodBank(string user, string pass) : username(user), password(pass) {}
    
    // Authentication method
    bool authenticate() const {
        string inputUser, inputPass;
        
        cout << "Enter username: ";
        cin >> inputUser;
        cout << "Enter password: ";
        cin >> inputPass;
        
        return (inputUser == username && inputPass == password);
    }
    
    // Add a new donor
    void addDonor() {
        Donor newDonor;
        newDonor.registerDonor();
        donors.push_back(newDonor);
    }
    
    // Add a new hospital
    void addHospital() {
        Hospital newHospital;
        newHospital.registerHospital();
        hospitals.push_back(newHospital);
    }
    
    // Record blood donation
    bool recordDonation() {
        if (donors.empty()) {
            cout << "No donors registered in the system!" << endl;
            return false;
        }
        
        string donorId;
        cout << "Enter Donor ID: ";
        cin >> donorId;
        
        // Find donor
        for (Donor &donor : donors) {
            if (donor.getId() == donorId) {
                // Check eligibility
                if (!donor.isEligibleToDonate()) {
                    cout << "Donor is not eligible to donate at this time." << endl;
                    return false;
                }
                
                // Record donation
                donor.recordDonation();
                
                // Create blood packet
                BloodPacket newPacket(donor.getBloodType(), donor.getRhesus(), donorId);
                inventory.push_back(newPacket);
                
                // Update inventory counts
                string bloodType = donor.getFullBloodType();
                if (bloodType == "A+") bloodCount.aPos++;
                else if (bloodType == "A-") bloodCount.aNeg++;
                else if (bloodType == "B+") bloodCount.bPos++;
                else if (bloodType == "B-") bloodCount.bNeg++; 
                else if (bloodType == "AB+") bloodCount.abPos++;
                else if (bloodType == "AB-") bloodCount.abNeg++;
                else if (bloodType == "O+") bloodCount.oPos++;
                else if (bloodType == "O-") bloodCount.oNeg++;
                
                cout << "Blood donation successfully recorded!" << endl;
                return true;
            }
        }
        
        cout << "Donor not found!" << endl;
        return false;
    }
    
    // Request blood for a hospital
    bool requestBlood() {
        if (hospitals.empty()) {
            cout << "No hospitals registered in the system!" << endl;
            return false;
        }
        
        string hospitalId;
        cout << "Enter Hospital ID: ";
        cin >> hospitalId;
        
        // Find hospital
        bool hospitalFound = false;
        for (Hospital &hospital : hospitals) {
            if (hospital.getId() == hospitalId) {
                hospitalFound = true;
                
                // Check payment
                hospital.checkPayment();
                if (!hospital.hasPayment()) {
                    cout << "Payment required before blood can be issued." << endl;
                    return false;
                }
                
                // Get blood type request
                string requestType, requestRhesus;
                cout << "Enter requested blood type (A, B, AB, O): ";
                cin >> requestType;
                cout << "Enter requested rhesus factor (+ or -): ";
                cin >> requestRhesus;
                
                string fullType = requestType + requestRhesus;
                
                // Find compatible blood packet
                for (BloodPacket &packet : inventory) {
                    if (!packet.isAllocated() && !packet.isExpired() && 
                        isCompatible(fullType, packet.getFullType())) {
                        
                        packet.allocate();
                        
                        // Update inventory counts
                        string packetType = packet.getFullType();
                        if (packetType == "A+") bloodCount.aPos--;
                        else if (packetType == "A-") bloodCount.aNeg--;
                        else if (packetType == "B+") bloodCount.bPos--;
                        else if (packetType == "B-") bloodCount.bNeg--; 
                        else if (packetType == "AB+") bloodCount.abPos--;
                        else if (packetType == "AB-") bloodCount.abNeg--;
                        else if (packetType == "O+") bloodCount.oPos--;
                        else if (packetType == "O-") bloodCount.oNeg--;
                        
                        cout << "Blood packet allocated successfully!" << endl;
                        packet.displayInfo();
                        return true;
                    }
                }
                
                cout << "No compatible blood packets available!" << endl;
                return false;
            }
        }
        
        if (!hospitalFound) {
            cout << "Hospital not found!" << endl;
        }
        return false;
    }
    
    // Check blood compatibility (recipient type, donor type)
    bool isCompatible(const string& recipient, const string& donor) const {
        // O- is universal donor
        if (donor == "O-") return true;
        
        // O+ can donate to any positive recipient
        if (donor == "O+" && recipient[recipient.length()-1] == '+') return true;
        
        // A- can donate to A and AB (both negative)
        if (donor == "A-" && (recipient == "A-" || recipient == "AB-")) return true;
        
        // A+ can donate to A+ and AB+
        if (donor == "A+" && (recipient == "A+" || recipient == "AB+")) return true;
        
        // B- can donate to B and AB (both negative)
        if (donor == "B-" && (recipient == "B-" || recipient == "AB-")) return true;
        
        // B+ can donate to B+ and AB+
        if (donor == "B+" && (recipient == "B+" || recipient == "AB+")) return true;
        
        // AB- can donate to AB-
        if (donor == "AB-" && recipient == "AB-") return true;
        
        // AB+ can donate to AB+
        if (donor == "AB+" && recipient == "AB+") return true;
        
        return false;
    }
    
    // Display inventory status
    void displayInventory() const {
        cout << "\n===== BLOOD BANK INVENTORY =====" << endl;
        cout << "A+  : " << bloodCount.aPos << " units" << endl;
        cout << "A-  : " << bloodCount.aNeg << " units" << endl;
        cout << "B+  : " << bloodCount.bPos << " units" << endl;
        cout << "B-  : " << bloodCount.bNeg << " units" << endl;
        cout << "AB+ : " << bloodCount.abPos << " units" << endl;
        cout << "AB- : " << bloodCount.abNeg << " units" << endl;
        cout << "O+  : " << bloodCount.oPos << " units" << endl;
        cout << "O-  : " << bloodCount.oNeg << " units" << endl;
        cout << "Total: " << 
            bloodCount.aPos + bloodCount.aNeg + 
            bloodCount.bPos + bloodCount.bNeg +
            bloodCount.abPos + bloodCount.abNeg +
            bloodCount.oPos + bloodCount.oNeg << " units" << endl;
        cout << "----------------------------------------" << endl;
    }
    
    // Display detailed inventory
    void displayDetailedInventory() const {
        cout << "\n===== DETAILED INVENTORY =====" << endl;
        if (inventory.empty()) {
            cout << "No blood packets in inventory." << endl;
            return;
        }
        
        int available = 0, allocated = 0, expired = 0;
        
        for (const BloodPacket &packet : inventory) {
            if (packet.isExpired()) {
                expired++;
            } else if (packet.isAllocated()) {
                allocated++;
            } else {
                available++;
                packet.displayInfo();
            }
        }
        
        cout << "\nSummary:" << endl;
        cout << "Available: " << available << " units" << endl;
        cout << "Allocated: " << allocated << " units" << endl;
        cout << "Expired: " << expired << " units" << endl;
        cout << "----------------------------------------" << endl;
    }
    
    // List all donors
    void listDonors() const {
        cout << "\n===== REGISTERED DONORS =====" << endl;
        if (donors.empty()) {
            cout << "No donors registered in the system." << endl;
            return;
        }
        
        for (const Donor &donor : donors) {
            donor.displayInfo();
        }
    }
    
    // List all hospitals
    void listHospitals() const {
        cout << "\n===== REGISTERED HOSPITALS =====" << endl;
        if (hospitals.empty()) {
            cout << "No hospitals registered in the system." << endl;
            return;
        }
        
        for (const Hospital &hospital : hospitals) {
            hospital.displayInfo();
        }
    }
    
    // Run the blood bank management system
    void run() {
        int loginAttempts = 0;
        const int MAX_ATTEMPTS = 3;
        
        cout << "\n\n";
    cout << "  ******************************\n";
    cout << "  *                            *\n";
    cout << "  *       BLOOD BANK           *\n";
    cout << "  *       MANAGEMENT           *\n";
    cout << "  *        SYSTEM              *\n";
    cout << "  *                            *\n";
    cout << "  ******************************\n\n";
        
        // Authentication
        while (loginAttempts < MAX_ATTEMPTS) {
            cout << "\n===== LOGIN =====" << endl;
            if (authenticate()) {
                cout << "Login successful!" << endl;
                break;
            } else {
                loginAttempts++;
                cout << "Invalid credentials. Attempts remaining: " 
                     << (MAX_ATTEMPTS - loginAttempts) << endl;
                
                if (loginAttempts >= MAX_ATTEMPTS) {
                    cout << "Too many failed attempts. System locked." << endl;
                    return;
                }
            }
        }
        
        // Main menu loop
        int choice;
        do {
            cout << "\n===== BLOOD BANK MANAGEMENT SYSTEM =====" << endl;
            cout << "1. Register New Donor" << endl;
            cout << "2. Register New Hospital" << endl;
            cout << "3. Record Blood Donation" << endl;
            cout << "4. Process Blood Request" << endl;
            cout << "5. View Blood Inventory" << endl;
            cout << "6. View Detailed Inventory" << endl;
            cout << "7. List All Donors" << endl;
            cout << "8. List All Hospitals" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    addDonor();
                    break;
                case 2:
                    addHospital();
                    break;
                case 3:
                    recordDonation();
                    break;
                case 4:
                    requestBlood();
                    break;
                case 5:
                    displayInventory();
                    break;
                case 6:
                    displayDetailedInventory();
                    break;
                case 7:
                    listDonors();
                    break;
                case 8:
                    listHospitals();
                    break;
                case 0:
                    cout << "Thank you for using the Blood Bank Management System!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 0);
    }
};

// Main function to run the program
int main() {
    // Create blood bank with admin credentials
    BloodBank bloodBank("admin", "password");
    
    // Run the system
    bloodBank.run();
    
    return 0;
}