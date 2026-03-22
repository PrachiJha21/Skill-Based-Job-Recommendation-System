# Skill-Based-Job-Recommendation-System
A console-based C++ application that intelligently matches job seekers with relevant jobs using skill-based weighted scoring and priority queue ranking. The system supports three user roles, Candidate, Employer, and Admin, and leverages efficient data structures like hashing, heaps, and binary search for optimal performance. Key features include smart job matching, skill gap analysis, profile strength calculation, and report generation, all backed by file-based persistence.
<br>
<ul><h2>Features</h2>
  <li>Three User Roles: Candidate, Employer, Admin</li>
  <li>Smart Matching Engine: Weighted Scoring based on required and optonal skills</li>
  <li>Priority Queue Ranking: Top jobs are ranked using a max-heap</li>
  <li>Profile Strength Calculation: Helps Candidate understand their completeness</li>
  <li>Reports and Analystics: Skill demand and candidate acitivity reports</li>
  <li>File Persistence: All data stored in files (Scalable to SQL in future)</li>
</ul>

<h2>Tech Stack</h2>
    <table>
        <thead>
            <tr><th>Concept</th><th>Implementation</th></tr>
        </thead>
        <tbody>
            <tr><td>Language</td><td>C++</td></tr>
            <tr><td>Hashing</td><td><code>unordered_map</code>, <code>unordered_set</code></td></tr>
            <tr><td>Priority Queue</td><td><code>priority_queue</code> (max-heap)</td></tr>
            <tr><td>Sorting & Searching</td><td><code>sort()</code>, binary search</td></tr>
            <tr><td>Data Structures</td><td>Vectors, strings</td></tr>
            <tr><td>File Handling</td><td><code>fstream</code> for persistent storage</td></tr>
        </tbody>
    </table>
<h2>System Architecture</h2>
  <pre>
┌─────────────────────────────────────┐
│         Authentication Layer        │
│   (Login / Registration using hash) │
└─────────────────────────────────────┘
                 │
┌─────────────────────────────────────┐
│         Core Application Layer      │
│  ┌─────────┐ ┌─────────┐ ┌───────┐  │
│  │Candidate│ │Employer │ │ Admin │  │
│  └─────────┘ └─────────┘ └───────┘  │
└─────────────────────────────────────┘
                 │
┌─────────────────────────────────────┐
│        Matching & Ranking Engine    │
│  (Weighted scoring + Priority Queue)│
└─────────────────────────────────────┘
                 │
┌─────────────────────────────────────┐
│         File Persistence Layer      │
│   (Users, Jobs, Applications data)  │
└─────────────────────────────────────┘
  </pre>

  <h2> Matching & Ranking Logic</h2>
<ol>
    <li><strong>Input Stage:</strong> Candidate logs in → skill set retrieved → system fetches all approved jobs.</li>
    <li><strong>Matching Stage (Weighted Skill Comparison):</strong> For each job, compare candidate skills with required skills (higher weight) and optional skills (lower weight). Compute a weighted match score.</li>
    <li><strong>Ranking Stage:</strong> Insert each job’s score into a priority queue (max‑heap). Extract top N highest scoring jobs.</li>
    <li><strong>Output Stage:</strong> Display ranked job list with match percentage. If score &lt; 100%, trigger skill gap analysis.</li>
</ol>
<h2>Skill Gap Analysis</h2>
<p>When a candidate views a job with match score between 60% and 100%:</p>
<ul>
    <li><strong>Input:</strong> Candidate skills + job required skills</li>
    <li><strong>Comparison:</strong> Set difference – for each required skill, check if it exists in candidate’s skill set.</li>
    <li><strong>Gap Detection:</strong> If missing skills are found, list them and suggest improvement.</li>
    <li><strong>Output:</strong> Display missing required skills and improvement suggestions.</li>
</ul>
