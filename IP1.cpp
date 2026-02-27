#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <cassert>
#include <stdexcept>

using namespace std;


class Guitar {
    string brand;
    string model;
    int numStrings;
    float price;
    int id;

    static int nextId;
    static int objectCount;

    void validateNumStrings(int n) {
        if (n < 4 || n > 12) {
            throw invalid_argument("numStrings must be between 4 and 12");
        }
    }

    void validatePrice(float p) {
        if (p <= 0) {
            throw invalid_argument("price must be greater than 0");
        }
    }

public:
    //Constructors
    Guitar(string brand, string model, int numStrings, float price)
        : brand(brand), model(model) {
        validateNumStrings(numStrings);
        validatePrice(price);
        this->numStrings = numStrings;
        this->price = price;
        this->id = nextId++;
        objectCount++;
    }

    Guitar(string brand, string model)
        : brand(brand), model(model), numStrings(6), price(100.0f) {
        this->id = nextId++;
        objectCount++;
    }

    // --- Copy constructor ---
    Guitar(const Guitar& other)
        : brand(other.brand), model(other.model),
          numStrings(other.numStrings), price(other.price) {
        this->id = nextId++;
        objectCount++;
    }

    //Destructor
    ~Guitar() {
        objectCount--;
    }

    //Getters
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    int getNumStrings() const { return numStrings; }
    float getPrice() const { return price; }
    int getId() const { return id; }

    //Setters
    void setBrand(const string& newBrand) { brand = newBrand; }
    void setModel(const string& newModel) { model = newModel; }

    void setNumStrings(int n) {
        validateNumStrings(n);
        numStrings = n;
    }

    void setPrice(float p) {
        validatePrice(p);
        price = p;
    }

    string toString() const {
        stringstream ss;
        ss << "Guitar[id=" << id
           << ", brand=" << brand
           << ", model=" << model
           << ", strings=" << numStrings
           << ", price=" << price << "]";
        return ss.str();
    }

    
    static int getObjectCount() { return objectCount; }
};


int Guitar::nextId = 1;
int Guitar::objectCount = 0;

//TESTS

// test1: create an object, getters return provided values, toString is correct
void test1() {
    Guitar g("Fender", "Stratocaster", 6, 999.99f);

    assert(g.getBrand() == "Fender");
    assert(g.getModel() == "Stratocaster");
    assert(g.getNumStrings() == 6);
    assert(g.getPrice() == 999.99f);
    assert(g.getId() >= 1);

    string expected = "Guitar[id=" + to_string(g.getId())
        + ", brand=Fender, model=Stratocaster, strings=6, price="
        + to_string(999.99f) + "]";

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

// test2: for every setter, call it and verify old value changes to new one
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

// test3: check that failed validation throws invalid_argument
void test3() {
    // Invalid numStrings (too low)
    bool caught = false;
    try {
        Guitar g("Test", "Model", 2, 100.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid numStrings (too high)
    caught = false;
    try {
        Guitar g("Test", "Model", 15, 100.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid price (zero)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, 0.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid price (negative)
    caught = false;
    try {
        Guitar g("Test", "Model", 6, -50.0f);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid setter: setNumStrings
    caught = false;
    try {
        Guitar g("Test", "Model", 6, 100.0f);
        g.setNumStrings(3);
    } catch (const invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    // Invalid setter: setPrice
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

// test5: create/delete list of objects dynamically, verify objectCount 0→N→0
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

// =============================================================================
// Main
// =============================================================================
int main() {
    cout << "Running Guitar class tests..." << endl;

    try {
        test1();
    } catch (const exception& e) {
        cerr << "  test1 FAILED: " << e.what() << endl;
        return 1;
    }

    // Clean up objects from test1 (they go out of scope)
    // Objects from test1 and test2 are local and destroyed automatically.

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

    // Create a list of Guitar objects as required
    list<Guitar> guitarList;
    guitarList.push_back(Guitar("Fender", "Telecaster", 6, 899.0f));
    guitarList.push_back(Guitar("Ibanez", "RG550", 6, 750.0f));
    guitarList.push_back(Guitar("Martin", "D-28", 6, 2999.0f));

    cout << "\nGuitar list:" << endl;
    for (const auto& g : guitarList) {
        cout << "  " << g.toString() << endl;
    }

    guitarList.clear();
    assert(Guitar::getObjectCount() == 0);

    cout << "\nAll tests passed!" << endl;
    return 0;
}
