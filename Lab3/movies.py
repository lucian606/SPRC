from flask import Flask, request, Response
from flask.json import jsonify
import json

movies = {}
current_id = 0

def addMovie(name):
    global movies
    global current_id
    current_id += 1
    new_movie = {'id': current_id, 'nume': name}
    movies[current_id] = new_movie


app = Flask(__name__)

@app.route('/')
def home():
    return'MOVIES Home Page'

@app.route('/movies', methods = ["GET", "POST"])
def get_movies():
    if request.method == "GET":
        lst = list(movies.values())
        return Response(json.dumps(lst), mimetype = 'application/json', status = 200)
    elif request.method == "POST":
        movie = request.json
        if movie.get("nume") == None:
            return Response(status = 400)
        else:
            name = movie["nume"]
            addMovie(name)
            return Response(status = 201)
    else:
        return Response(status = 400)


@app.route('/movie/<int:id>', methods = ["GET", "PUT", "DELETE"])
def movie(id):
    global movies
    if request.method == "GET":
        if movies.get(id) == None:
            return Response(status = 404)
        return Response(json.dumps(movies[id]), mimetype = 'application/json', status = 200)
    elif request.method == "PUT":
        if movies.get(id) == None:
            return Response(status = 404)
        movie = request.json
        if movie.get('nume') == None:
            return Response(status = 400)
        name = movie.get('nume')
        movies[id]['nume'] = name
        return Response(status = 200)
    elif request.method == "DELETE":
        if movies.get(id) == None:
            return Response(status = 404)
        movies.pop(id)
        return Response(status = 200)

@app.route('/reset', methods = ["DELETE"])
def reset_movies():
    global movies
    global current_id
    movies = {}
    current_id = 0
    return Response(status = 200)
    

if __name__ == '__main__':
    app.run('0.0.0.0', port = 7030, debug = True)
