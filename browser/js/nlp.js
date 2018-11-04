const AUX_VERBS = [
  'do',
  'does',
  'be',
  'can',
  'could',
  'dare',
  'have',
  'may',
  'might',
  'must',
  'need',
  'ought',
  'shall',
  'should',
  'will',
  'would'
];

function GetVerbs (msg) {
  msg = nlp(msg).out('root');
  var doc = nlp(msg);
  var verbs = doc.verbs();
  var tags = verbs.out('tags');
  var verbs_to_return = [];
  
  console.log(tags);
  for (var i in tags) {
    if (!tags[i].tags.includes('Auxiliary') &&
        !AUX_VERBS.includes(tags[i].normal)) {
      verbs_to_return.push(tags[i].normal);
    }
  }
  console.log(verbs_to_return);
}
