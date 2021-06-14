from flask import Flask, render_template, request, redirect, url_for, session, jsonify
from flask_mysqldb import MySQL
import hashlib
from functools import wraps
app = Flask(__name__)


app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = '1234'
app.config['MYSQL_DB'] = 'deneme'
app.secret_key = "cmpe321"
mysql = MySQL(app)

def login_required(f):
    @wraps(f)
    def wrap(*args, **kwargs):
        if 'logged_in' in session:
            return f(*args, **kwargs)
        else:
            return 'you need to login first'
    return wrap

@app.route('/', methods=['GET'])
def firstpage():
    return render_template('firstpage.html')

@app.route('/user', methods=['GET', 'POST'])
def user():
    error = None
    if request.method == 'POST':
        details = request.form
        username = details['username']
        password = details['password']
        password = hashlib.sha256(password.encode('utf-8')).hexdigest()
        institution = details['institution']
        cur = mysql.connection.cursor()
        cur.execute("SELECT password FROM user WHERE username=%s and institution=%s", (username, institution,))
        data = cur.fetchall()
        mysql.connection.commit()
        cur.close()
        if not data:
            error = 'invalid username or password'
        else:
            if data[0][0] == password:
                session['logged_in'] = True
                return redirect(url_for('userlogged'))
            else:
                error = 'invalid username or password'
    return render_template('user.html', error=error)
@app.route('/databasemanager', methods=['GET', 'POST'])
def databasemanager():
    error = None
    if request.method == 'POST':
        details = request.form
        username = details['username']
        password = details['password']
        password = hashlib.sha256(password.encode('utf-8')).hexdigest()
        cur = mysql.connection.cursor()
        cur.execute("SELECT password FROM databasemanager WHERE username=%s",(username,))
        data =cur.fetchall()
        mysql.connection.commit()
        cur.close()
        if not data:
            error = 'invalid username or password'
        else:
            if data[0][0] == password:
                session['logged_in'] = True
                return redirect(url_for('dbmanager'))
            else:
                error = 'invalid username or password'
    return render_template('databasemanager.html', error=error)

@app.route('/logout')
@login_required
def logout():
    session.pop('logged_in', None)
    return redirect(url_for('firstpage'))

@app.route('/databasemanager/logged', methods = ['GET', 'POST','PUT','DELETE'])
@login_required
def dbmanager():
    error = None
    if request.method == 'POST':
        act = request.form['action']
        if act == 'delete drug':
            details = request.form
            drugbankid = details['drugbank_id']
            cur = mysql.connection.cursor()
            cur.execute("SELECT * FROM drugs WHERE drugbank_id=%s", (drugbankid, ))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            if not data:
                error = 'invalid user'
            else:
                cur = mysql.connection.cursor()
                cur.execute("DELETE FROM drugs WHERE drugbank_id = %s", (drugbankid,))
                mysql.connection.commit()
                cur.close()
                return 'success'
        if act == 'delete protein':
            details = request.form
            uniprot = details['uniprot_id']
            cur = mysql.connection.cursor()
            cur.execute("SELECT * FROM proteins WHERE uniprot_id=%s ", (uniprot, ))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            if not data:
                error = 'invalid protein'
            else:
                cur = mysql.connection.cursor()
                cur.execute("DELETE FROM proteins WHERE uniprot_id = %s", (uniprot,))
                mysql.connection.commit()
                cur.close()
                return 'success'
        if act == 'add user':
            details = request.form
            username = details['username']
            password = details['password']
            name = details['name']
            password = hashlib.sha256(password.encode('utf-8')).hexdigest()
            institution = details['institution']
            cur = mysql.connection.cursor()
            cur.execute("SELECT username,name, institution FROM user WHERE username=%s and institution=%s", (username, institution,))
            data = cur.fetchall()
            if not data:
                cur.execute("INSERT INTO user(username, institution, password, name) VALUES(%s, %s, %s, %s)", (username, institution, password,name,))
                cur.execute("select username,name, institution from user WHERE username = %s and institution=%s", (username,institution))
                data = cur.fetchall()
                x = [['username', 'name', 'institution']]
                x += data
                mysql.connection.commit()
                cur.close()
                return render_template('seeDrugs.html', data=x)
            else:
                error = 'user already registered'
        if act == 'change affinity':
            drug_details = request.form
            reaction_id = drug_details['reaction_id']
            affinity = drug_details['affinity']
            cur = mysql.connection.cursor()
            cur.execute("UPDATE reaction SET affinity_nM = %s WHERE reaction_id = %s", (affinity,reaction_id,))
            cur.execute("select drugbank_id, reaction_id,uniprot_id,affinity_nM,target_name,measure,doi from reaction WHERE reaction_id = %s", ( reaction_id,))
            data=cur.fetchall()
            x = [['drugbank_id', 'reaction_id', 'uniprot_id', 'affinity_nM', 'target_name', 'measure', 'doi']]
            x += data
            mysql.connection.commit()
            cur.close()
            return render_template('seeDrugs.html', data=x)
        if act == 'see drugs':
            cur = mysql.connection.cursor()
            cur.execute("select d.drugbank_id, d.name, d.description, group_concat(drug2 separator ', ') \
                            from drugs d left join interactions on d.drugbank_id = drug1 group by d.drugbank_id")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank_id','name','description', 'drug_interactions']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'update contributors':
            details = request.form
            reaction_id = details['reaction_id']
            return redirect(url_for('update_contributors',variable=reaction_id))
        if act == 'see side effects':
            data = select_sideeffect()
            return render_template('seeDrugs.html', data=data)
        if act == 'see contributors':
            cur = mysql.connection.cursor()
            cur.execute("select article.doi, group_concat(name separator '; '), article.institution from article join contributors on  article.doi = contributors.doi join user u on contributors.username = u.username and u.institution=article.institution group by doi;")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['doi', 'authors', 'institute']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see users':
            cur = mysql.connection.cursor()
            query = """Select username, institution, name from user"""
            cur.execute(query)
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['username', 'institution', 'name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_drug_target_interaction':
            cur = mysql.connection.cursor()
            query = """select drugbank_id, reaction_id,uniprot_id,affinity_nM,target_name,measure,doi from reaction"""
            cur.execute(query)
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank_id', 'reaction_id', 'uniprot_id', 'affinity_nM', 'target_name', 'measure', 'doi']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see proteins':
            data = select_protein()
            return render_template('seeDrugs.html', data=data)
    return render_template('dbmanager_console.html', error=error)


@app.route('/user/logged', methods=['GET', 'POST', 'PUT', 'DELETE'])
@login_required
def userlogged():
    if request.method == 'POST':
        act = request.form['action']
        if act == 'see_drugs':
            data = select_drug()
            return render_template('seeDrugs.html', data=data)
        if act == 'see_interaction_of_drug':
            details = request.form
            drug_id = details['drug_id']
            cur = mysql.connection.cursor()
            cur.execute("select drug2, name from interactions join drugs d on interactions.drug2 = d.drugbank_id where drug1= %s",(drug_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_side_effects_of_drug':
            details = request.form
            drug_id = details['drug_id']
            cur = mysql.connection.cursor()
            cur.execute(
                "select side_effect_name, h.umls_cui from side_effect join has h on side_effect.umls_cui = h.umls_cui where drugbank_id like %s",
                (drug_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['side effect name', 'umls_cui']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_interacting_targets_of_drug':
            details = request.form
            drug_id = details['drug_id']
            cur = mysql.connection.cursor()
            cur.execute(
                "select uniprot_id, target_name from reaction where drugbank_id like %s", (drug_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['uniprot id', 'target name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_interacting_drugs_of_protein':
            details = request.form
            protein_id = details['protein_id']
            cur = mysql.connection.cursor()
            cur.execute(
                "select drugbank_id, name from drugs where drugbank_id in (select drugbank_id from reaction where uniprot_id like %s)",
                (protein_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_drugs_that_affect_same_protein':
            cur = mysql.connection.cursor()
            cur.execute("select proteins.uniprot_id, group_concat(distinct drugbank_id separator ', ') from proteins left join reaction on proteins.uniprot_id=reaction.uniprot_id group by uniprot_id")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['uniprot id', 'drugbank ids']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_proteins_that_bind_the_same_drug':
            cur = mysql.connection.cursor()
            cur.execute("select drugs.drugbank_id, group_concat(distinct uniprot_id separator ', ') from drugs left join reaction r on drugs.drugbank_id = r.drugbank_id group by drugs.drugbank_id")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'uniprot ids']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'see_drugs_that_have_specifc_side_effect':
            details = request.form
            side_effect_id = details['side_effect_id']
            cur = mysql.connection.cursor()
            cur.execute(
                "select drugbank_id, name from drugs where drugbank_id in (select drugbank_id from has where umls_cui like %s)",
                (side_effect_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'drug_with_keyword':
            details = request.form
            keyword = details['keyword']
            cur = mysql.connection.cursor()
            cur.execute(
                "select drugbank_id, name, description from drugs where description like %s", ("%" + keyword + "%",))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'name', 'description']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'least_side_effect':
            details = request.form
            protein_id = details['protein_id']
            cur = mysql.connection.cursor()
            cur.execute("""select drugs.drugbank_id, name from drugs join has h on drugs.drugbank_id = h.drugbank_id
                            join reaction r on drugs.drugbank_id = r.drugbank_id where r.uniprot_id like %s
                            group by drugs.drugbank_id having count(*) = (select min(c) from (select count(*) c from drugs 
                            join has hh on drugs.drugbank_id = hh.drugbank_id join reaction rr on drugs.drugbank_id = rr.drugbank_id 
                            where rr.uniprot_id like %s group by drugs.drugbank_id) x)""", (protein_id, protein_id,))
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['drugbank id', 'name']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'articles':
            cur = mysql.connection.cursor()
            cur.execute("select article.doi, group_concat(name separator '; ') from article join contributors on \
             article.doi = contributors.doi join user u on contributors.username = u.username and u.institution=article.institution group by doi;")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['DOI', 'authors']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'scores':
            cur = mysql.connection.cursor()
            cur.execute("select * from institutions order by score desc")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['institution', 'scores']]
            x += data
            return render_template('seeDrugs.html', data=x)
        if act == 'filter':
            details = request.form
            drug_id = details['drug_id']
            measurement_type = details['measurement_type']
            low_affinity = details['low_affinity']
            high_affinity = details['high_affinity']
            cur = mysql.connection.cursor()
            args = (drug_id,measurement_type,low_affinity,high_affinity)
            cur.execute("call filter(%s,%s,%s,%s)",args)
            data_uni = cur.fetchall()
            data = data_uni
            #mysql.connection.commit()
            cur.close()
            x = [['uniprot_id', 'target_name']]
            x += data
            return render_template('seeDrugs.html', data=x)

    return render_template('user_console.html')
@app.route('/databasemanager/logged/<variable>', methods=['GET', 'POST'])
@login_required
def update_contributors(variable):
    cur = mysql.connection.cursor()
    cur.execute(
        "set @tempr =(select reaction.doi from reaction WHERE reaction_id = %s)",
        (variable,))
    cur.execute(
        "select doi, group_concat(name separator '; '), institution from contributors join user u on contributors.username = u.username where doi = @tempr;")
    data = cur.fetchall()
    mysql.connection.commit()
    cur.close()
    x = [['doi', 'authors', 'institution']]
    x += data
    if request.method == 'POST':
        act = request.form['action']
        if act =='add contributor':
            details = request.form
            username = details['username']
            name = details['author name']
            password = details['password']
            cur = mysql.connection.cursor()
            cur.execute(
                "set @tempinst = (select institution from article where doi in (select doi from reaction where reaction_id like %s))",
                (variable,))
            cur.execute("SELECT * FROM user WHERE username=%s and institution= @tempinst", (username, ))
            data = cur.fetchall()
            if not data:
                password = hashlib.sha256(password.encode('utf-8')).hexdigest()
                cur.execute("INSERT INTO user(username, institution, password, name) VALUES(%s, @tempinst, %s, %s)",
                            (username, password, name,))

            cur.execute("select doi from contributors where username =%s and doi in (select doi from reaction where reaction_id like %s)",
                            (username, variable,))
            data = cur.fetchall()
            if not data:
                cur.execute("INSERT INTO contributors(username, doi) VALUES(%s, (select doi from reaction where reaction_id like %s))",
                            (username,variable))
                cur.execute(
                    "set @tempr =(select reaction.doi from reaction join article on article.doi=reaction.doi  WHERE reaction_id = %s)",
                    (variable,))
                cur.execute(
                    "select doi, group_concat(name separator '; '), institution from contributors join user u on contributors.username = u.username where doi = @tempr;")
                data = cur.fetchall()
                x = [['doi', 'authors', 'insitution']]
                x += data
                mysql.connection.commit()
                cur.close()

            else:
                return render_template('updateContributors.html', data=x,error="already contributor")
        if act =='delete contributor':
            details = request.form
            username = details['username']
            name = details['author name']
            password = details['password']
            cur = mysql.connection.cursor()
            cur.execute(
                "select doi from contributors where username =%s and doi in (select doi from reaction where reaction_id like %s)",
                (username, variable,))
            data = cur.fetchall()
            if not data:
                return render_template('updateContributors.html', data=x, error="not a contributor")
            cur.execute(
                "select username from user where username =%s and name = %s",
                (username, name,))
            data = cur.fetchall()
            if not data:
                return render_template('updateContributors.html', data=x, error="wrong name")
            cur.execute("DELETE from contributors where username = %s and doi in (select doi from reaction where reaction_id like %s)",(username,variable))
            cur = mysql.connection.cursor()
            cur.execute(
                "set @tempr =(select reaction.doi from reaction join article on article.doi=reaction.doi  WHERE reaction_id = %s)",
                (variable,))
            cur.execute(
                "select doi, group_concat(name separator '; '), institution from contributors join user u on contributors.username = u.username where doi = @tempr;")
            data = cur.fetchall()
            mysql.connection.commit()
            cur.close()
            x = [['doi', 'authors', 'insitution']]
            x += data
    return render_template('updateContributors.html', data=x)

def select_protein():
    cur = mysql.connection.cursor()

    query = """Select * from proteins"""
    cur.execute(query)
    data = cur.fetchall()

    mysql.connection.commit()
    cur.close()
    x = [['uniprot id', 'sequence']]
    x += data
    return x
def select_sideeffect():
    cur = mysql.connection.cursor()

    query = """Select * from side_effect"""
    cur.execute(query)
    data = cur.fetchall()

    mysql.connection.commit()
    cur.close()
    x = [['umls_cui', 'name']]
    x += data
    return x

def select_drug():
    cur = mysql.connection.cursor()

    query = """Select did, name, description, smiles, GROUP_CONCAT(target_name SEPARATOR ',  '), sefname from 
                (SELECT drugs.drugbank_id as did, name,description,smiles,GROUP_CONCAT(side_effect_name SEPARATOR ',   ') as sefname
                from drugs left join has b on drugs.drugbank_id = b.drugbank_id
                left join side_effect sag on b.umls_cui = sag.umls_cui group by  did) as bbb
                left join reaction on reaction.drugbank_id = bbb.did group by did"""
    cur.execute(query)
    data = cur.fetchall()

    mysql.connection.commit()
    cur.close()
    x = [['drug id','name','description','smiles','target names','side effects']]
    x+=data
    return x

if __name__ == '__main__':
    app.run()

