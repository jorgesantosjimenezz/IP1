#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <cassert>
#include <stdexcept>

using namespace std;

class Guitar {
private:

    string brand;
    string model;
    int numStrings;
    float price;

public:
    int id;

private:

    static int nextId;
    static int objectCount;

    // --- Validation methods ---
    void validateNumStrings(int n);
    void validatePrice(float p);

public:
    // --- Constructors ---
    Guitar(string brand, string model){
        setBrand(brand);
        setModel(model);
        setNumStrings(6);
        setPrice(100.0f);
        this->id = nextId++;
        objectCount++;
    }

    Guitar(string brand, string model, int numStrings, float price){
        setBrand(brand);
        setModel(model);
        setNumStrings(numStrings);
        setPrice(price);
        this->id = nextId++;
        objectCount++;
    }

    // --- Copy constructor ---
    Guitar(const Guitar& other){
        setBrand(other.brand);
        setModel(other.model);
        setNumStrings(other.numStrings);
        setPrice(other.price);
        this->id = nextId++;
        objectCount++;
    }

    // --- Destructor ---
    ~Guitar() {
    objectCount--;
    }

    // --- Getters ---
    string getBrand() { return brand; }
    string getModel() { return model; }
    int getNumStrings() { return numStrings; }
    float getPrice() { return price; }
    int getId() { return id; }

    // --- Setters ---
    void setBrand(string brand) { this->brand = brand; }
    void setModel(string model) { this->model = model; }
    void setNumStrings(int numStrings) {
        validateNumStrings(numStrings);
        this->numStrings = numStrings;
    }
    void setPrice(float price) {
        validatePrice(price);
        this->price = price;
    }

    
    string toString() {
    stringstream ss;
    ss << "Guitar[id=" << getId()
       << ", brand=" << getBrand()
       << ", model=" << getModel()
       << ", strings=" << getNumStrings()
       << ", price=" << getPrice() << "]";
    return ss.str();
}

    // --- Static methods ---
    static int getObjectCount(){return objectCount;};
};

// --- Static member initialization ---
int Guitar::nextId = 1;
int Guitar::objectCount = 0;

// --- Validation methods ---
void Guitar::validateNumStrings(int n) {
    if (n < 4 || n > 12) {
        throw invalid_argument("NumStrings must be between 4 and 12.");
    }
}

void Guitar::validatePrice(float p) {
    if (p <= 0) {
        throw invalid_argument("Price must be greater than 0.");
    }
}


// --- TESTS ---

// test1: create an object getters return the provided values and toString returns what it must
void test1() {
    Guitar g("Fender", "Stratocaster", 6, 999.99f);

    assert(g.getBrand() == "Fender");
    assert(g.getModel() == "Stratocaster");
    assert(g.getNumStrings() == 6);
    assert(g.getPrice() == 999.99f);
    assert(g.getId() >= 1);

    // Verify toString contains all relevant info
    string s = g.toString();
    assert(s.find("Fender") != string::npos);
    assert(s.find("Stratocaster") != string::npos);
    assert(s.find("Guitar[id=") != string::npos);

    // Test second constructor (defaults)
    Guitar g2("Gibson", "Les Paul");
    assert(g2.getBrand() == "Gibson");
    assert(g2.getModel() == "Les Paul");
    assert(g2.getNumStrings() == 6);
    assert(g2.getPrice() == 100.0f);

    cout << "  test1 passed" << endl;
}

// test2: for every public setter call it and check whether the old value properly changes into a new one
void test2() {
    Guitar g("Yamaha", "FG800", 6, 200.0f);

    // setBrand
    assert(g.getBrand() == "Yamaha");
    g.setBrand("Taylor");
    assert(g.getBrand() == "Taylor");

    // setModel
    assert(g.getModel() == "FG800");
    g.setModel("214ce");
    assert(g.getModel() == "214ce");

    // setNumStrings
    assert(g.getNumStrings() == 6);
    g.setNumStrings(12);
    assert(g.getNumStrings() == 12);

    // setPrice
    assert(g.getPrice() == 200.0f);
    g.setPrice(350.5f);
    assert(g.getPrice() == 350.5f);

    cout << "  test2 passed" << endl;
}

// test3: check if failed validation throws and exception of the desired type
void test3() {
    // Invalid numStrings (2)
    bool caught = false;
    try {
        Guitar g("Test", "Model", 2, 100.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid numStrings (15)
    caught = false;
    try {
        Guitar g("Test", "Model", 15, 100.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid price (0)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, 0.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid price (-50)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, -50.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid setter: setNumStrings (2)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, 100.0f);
        g.setNumStrings(2);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid setter: setPrice (-10)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, 100.0f);
        g.setPrice(-10.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    cout << "  test3 passed" << endl;
}

// test4: check whether automatic numbering works properly (id can only increase)
void test4() {
    Guitar g1("A", "A1", 6, 50.0f);
    Guitar g2("B", "B1", 7, 75.0f);
    Guitar g3("C", "C1", 8, 100.0f);

    assert(g1.getId() < g2.getId());
    assert(g2.getId() < g3.getId());

    // Dynamic object also gets a higher id
    Guitar* g4 = new Guitar("D", "D1", 4, 25.0f);
    assert(g3.getId() < g4->getId());
    delete g4;

    cout << "  test4 passed" << endl;
}

/* test5: create a list of objects for the class dynamically (using new and later delete), and make sure the
count of objects in memory is valid (0 in the beginning, increases with new and decreases with delete,
and 0 in the end)*/
void test5() {
    int countBefore = Guitar::getObjectCount();
    assert(countBefore == 0);

    const int N = 5;
    Guitar** guitars = new Guitar*[N];

    // Create N objects
    for (int i = 0; i < N; i++) {
        guitars[i] = new Guitar("Brand" + to_string(i), "Model" + to_string(i),
                                6, 50.0f + i * 10);
        assert(Guitar::getObjectCount() == countBefore + i + 1);
    }

    assert(Guitar::getObjectCount() == N);

    // Delete all objects
    for (int i = 0; i < N; i++) {
        delete guitars[i];
        assert(Guitar::getObjectCount() == N - i - 1);
    }

    delete[] guitars;
    assert(Guitar::getObjectCount() == 0);

    cout << "  test5 passed" << endl;
}

// --- MAIN ---

int main() {
    cout << "Running Guitar class tests..." << endl;

    try {
        test1();
    } catch (const exception& e) {
        cerr << "  test1 FAILED: " << e.what() << endl;
        return 1;
    }

    try {
        test2();
    } catch (const exception& e) {
        cerr << "  test2 FAILED: " << e.what() << endl;
        return 1;
    }

    try {
        test3();
    } catch (const exception& e) {
        cerr << "  test3 FAILED: " << e.what() << endl;
        return 1;
    }

    try {
        test4();
    } catch (const exception& e) {
        cerr << "  test4 FAILED: " << e.what() << endl;
        return 1;
    }

    // test5 needs objectCount == 0, so we run it last after all stack objects are gone
    try {
        test5();
    } catch (const exception& e) {
        cerr << "  test5 FAILED: " << e.what() << endl;
        return 1;
    }



    cout << "\nAll tests passed!" << endl;
    return 0;
}
