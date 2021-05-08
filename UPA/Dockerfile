FROM python:3.8-slim-buster

RUN mkdir /app
WORKDIR /app

COPY requirements.txt /app/

## install dependencies
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y netcat-openbsd gcc && \
    apt-get clean

RUN pip install --upgrade pip && \
    pip install --no-cache-dir -r requirements.txt && \
    pip freeze

COPY ./upa/ /app/upa
COPY runner.py /app
COPY index.html /app

ENV PYTHONPATH="$PYTHONPATH:/app"

EXPOSE 8000

CMD uvicorn --host 0.0.0.0 upa.utils.shonte:app --reload
