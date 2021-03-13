# C-Language-Acid-Rain
Typing game acid rain made in C language.

# 설명
C언어로 제작한 산성비 게임입니다.<br>
C언어를 배우고자 하는 의도로 접근했습니다.<br>
당시 참고한 프로젝트의 버전이 오래되어 그래픽의 위치가 정확하지 않고, 보안 오류가 많이 발생했습니다.<br>
모든 코드를 이해하려고 노력했고 보완할 부분은 고치고 새로운 요소를 추가했습니다.<br>

# 환경
- 기간: 2020-10-?? ~ 2020-12-18<br>
- OS: Windows 10<br>
- Language: C<br>
- Tools: Visual Studio 2019 (ver 16.8.2)<br>

# 수정, 추가 요소
- 보안 오류 해결<br>
- 깨져보이는 그래픽을 깔끔하게 변경<br>
- 메인 화면, 엔딩 화면, 다음 스테이지 화면, 클리어 화면 추가<br>
- 화면 크기를 설정하여 모든 환경에서 일관되게 변경<br>
- 스테이지마다 랜덤한 음악 추가, 연속으로 같은 음악 나오지 않게 설정<br>
- 불필요한 코드 제거 및 코드 간략화, 모든 코드에 주석을 자세하게 추가<br>
- (필요는 없지만) 동적 메모리 해제 추가<br>
- (특별한 입력으로 들어가는) 점수 제한이 없는 무한모드 추가<br>
- 단어장에 개수 약 30개에서 1000개로 정도로 변경<br>
- 게임 난이도 관련 변수를 위쪽에 정리<br>

# 오류
- (메모리 읽기 에러) 무한모드에서 단어 낙하 주기를 빠르게 하면 튕김.<br>
- (메모리 읽기 에러) 원인을 알 수 없는 튕김. 단어 리스트 관련으로 추측.<br>

# 참고
참고한 프로젝트: https://github.com/parkseulkee/typing_game<br>
단어장 단어: https://www.ef.com/wwen/english-resources/english-vocabulary/top-1000-words/<br>
음악: https://www.mewpot.com/<br>
MP3-WAV 변환하는 사이트: https://convertio.co/kr/mp3-wav/<br>
