
template <typename Subject>
class CritHit {

public:
    CritHit(Subject& _subject, int penalty) : subject(_subject), penaltyApplied(penalty) {
        subject.SetAccuracy(subject.GetAccuracy() - penaltyApplied);
        //screenshake
    }

    ~CritHit() = default;

private:
    Subject& subject;
    int penaltyApplied;

//screenshake
};