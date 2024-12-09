Ανάπτυξη Εφαρμογών για Πληροφοριακά Συστήματα - 2ο Παραδοτέο

Κωνσταντίνος Αραστσιάρης - 1115202100006
Φίλιππος Τσιρχόγλου - 1115202100197

Ανάθεση εργασιών:

      Κατά την δημιουργία του 2ου παραδοτέου, αν και εργαζόμασταν συχνά μαζί μέσω διά ζώσης συναντήσεων, ο διαχωρισμός ήταν πιο ξεκάθαρος,
    με τον καθένα να υλοποιεί στο δικό του branch και έπειτα κάνοντας push στο main branch με την χρήση pull requests. Συνεργατικά έγινε 
    το debbuging και συχνά υπήρχαν επεμβάσεις του ενός στις αναθέσεις του άλλου για τυχόν διορθώσεις. Τα κομμάτια που ασχολήθηκε ο καθένας
    είναι τα παρακάτω:

    - Κωνσταντίνος Αραστσιάρης: 

            - Υλοποίηση μέρους της main
            - Υλοποίηση συναρτήσεων για το δίαβασμα datasets, queries ,groundtruth και configuration file
            - Υλοποίηση της συνάρητησης filteredGreedySearch
            - Υλοποίηση συναρτήσεων find_filter_set και vectors_equal
            - Μετατροπή groundtruth σε binary file
    
    - Φίλιππος Τσιρχόγλου:

            - Υλοποίηση μέρους της main
            - Υλοποίηση συναρτήσεων filteredFindMedoid, filteredRobustPrune, filteredVamana
            - Υλοποίηση αλγορίθμου StitchedVamana
            - Unit testing

    
    Εκτέλεση: 

          Βάζουμε τις παραμέτρους στο config.txt, make και make run. Με make tests εκτελούνται τα tests και με make val εκτελούνται 
        τα tests με την χρήση valgrind.

        Με F0 στο πεδίο METHOD-TYPE του config.txt εκτελείται η συνάρτηση filteredVamana για type 0 queries και με F1 για type 1.
        Με S0 εκτελέιται η StitchedVamana για type 0 queries και με S1 για type 1.