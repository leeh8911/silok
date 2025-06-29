# 📝 실록 (Silok)

> **실록**은 사용자의 일상과 업무 기록을 정리하고, 연결하고, 되돌아보게 돕는 기록 기반 메모 시스템입니다.  
> 태그, 프로젝트, LLM 기반 링크 추천을 통해 생각을 자연스럽게 확장할 수 있도록 설계되었습니다.

---

## 📌 주요 기능

- 노트, 태그, 프로젝트 기반의 구조화된 기록 관리
- 노트 ↔ 태그, 노트 ↔ 프로젝트 기반 필터링 및 분류
- 노트 간의 참조와 링크 연결 기능
- **Markdown** 기반의 노트 작성 및 렌더링
- **LLM 기반 자연어 검색** 및 자동 링크 추천
- JWT 기반 사용자 인증 및 계정 관리

---

## 🧠 주요 사용자

- 기록 습관을 기르고 싶은 사람
- 프로젝트나 일정을 체계적으로 관리하고 싶은 사람
- 주의 집중력이 부족해 기록으로 극복하고자 하는 사람

---

## 🧰 기술 스택

| 항목       | 내용                      |
|------------|---------------------------|
| 언어       | C++20                     |
| 빌드       | CMake                     |
| 데이터베이스 | SQLite                     |
| 인증       | JWT (HS256)               |
| 서버 환경  | Ubuntu 22.04              |
| API        | RESTful API               |
| 주요 라이브러리 | spdlog, fmt, jwt-cpp, sqlite_orm |

---

## ⚙️ 설치 및 실행

### 1. Docker로 실행

```bash
sudo docker build -t silok-env .
sudo docker run -it -v $(pwd):/workspace --name silok-dev silok-env:latest
```

### 2. 로컬 개발 환경

#### 클론

```bash
git clone https://github.com/your-username/silok.git
cd silok
```

#### 빌드

```bash
mkdir build && cd build
cmake .. -DBUILD_TEST=ON -DBUILD_DOCS=ON
make
```

---

## 📁 프로젝트 구조

```text
src/
├── domain/            # 도메인 모델 및 인터페이스 (ex. Note, Tag, Project)
├── infra/             # DB, JWT, 로그 등 외부 구현체
├── service/           # (예정) 유스케이스 계층
└── api/               # (예정) REST API 핸들러
```

---

## 🚀 향후 계획

- [ ] 모바일 앱 지원 (React Native 또는 Flutter)
- [ ] WebSocket 기반 실시간 동기화
- [ ] 사용자별 템플릿 추천 기능
- [ ] LLM 기능 고도화 (요약, 유사 노트 추천, 클러스터링 등)

---

## 🔒 보안

- 모든 API는 JWT 기반 인증으로 보호됩니다.
- 사용자 데이터는 로컬 SQLite에 저장되며 외부로 전송되지 않습니다.
- LLM 기능은 외부 API가 아닌 자체 서버에서 로컬로 수행됩니다.

---

## 📄 라이선스

MIT License

---

## 🙌 기여

기여를 환영합니다!  
기능 제안, 버그 수정, 테스트 코드 추가 등 자유롭게 PR을 보내주세요.  
자세한 사항은 `CONTRIBUTING.md` (작성 예정)를 참고해주세요.

---

## ✨ 참고

- **‘실록’**은 조선왕조실록처럼, 개인의 일상을 체계적으로 정리하여 기록으로 남긴다는 의미에서 출발했습니다.
- 향후 README에는 API 문서, DB 스키마, 기능 상세 플로우도 함께 추가될 예정입니다.
