# 1단계: Build 단계
FROM node:20 AS builder

WORKDIR /app

# package.json과 lock 파일을 복사하고 의존성 설치
COPY package*.json ./
RUN npm install

# 소스 복사 및 빌드
COPY . .
RUN npm run build

# 2단계: 실제 배포용 이미지
FROM nginx:alpine

# Nginx 설정 복사 (필요 시)
# COPY nginx.conf /etc/nginx/nginx.conf

# 빌드된 파일 복사
COPY --from=builder /app/dist /usr/share/nginx/html

# 포트 오픈
EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
