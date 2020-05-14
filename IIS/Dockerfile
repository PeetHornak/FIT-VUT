FROM python:3.7.5-alpine

WORKDIR /app

COPY ./dst/default.jpg /images/

RUN apk add --no-cache postgresql-libs && \
    apk add --no-cache --virtual .build-deps gcc musl-dev postgresql-dev

COPY ./requirements.txt /app/requirements.txt

RUN python3 -m pip install -r requirements.txt --no-cache-dir && \
    apk --purge del .build-deps

COPY ./fitcado /app/fitcado

ENV PYTHONPATH="$PYTHONPATH:/app"
ENTRYPOINT ["python"]
CMD ["fitcado/run.py"]
