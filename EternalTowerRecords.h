enum ETowerAttemptType {
	enumTOWER_ATTEMPT_SOLO,
	enumTOWER_ATTEMPT_PARTY,
};

struct TowerCharacterAttempt {
	int attemptId;
	int characterId;
	int level;
	int job;
  string characterName;
};

struct TowerAttemptRecord {
  int leaderboardPosition;
  int attemptId;
  int maxFloor;
  ETowerAttemptType attemptType;
  vector<TowerCharacterAttempt*> players;
};

