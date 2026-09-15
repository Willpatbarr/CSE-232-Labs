# CSE 232 Labs

Shared repo for CSE 232 (Data Structures) lab work.

Upstream: `https://github.com/Willpatbarr/CSE-232-Labs`

---

## Repo layout

Each lab gets one folder. Inside are the three starter zips the course
ships — the 85%, 100%, and 115% versions. Unzip the one you decide to do,
*in that folder*, when you start the lab. Leave the zips committed.

```
Lab02-Vector/          232.02.Lab.85.zip   232.02.Lab.100.zip   232.02.Lab.115.zip
Lab03-Stack/           232.03.Lab.85.zip   232.03.Lab.100.zip   232.03.Lab.115.zip
Lab04-LinkedList/      ...
Lab05-List/
Lab06-BinaryTree/
Lab07-BST/
Lab08-Set/
Lab09-Map/
Lab10-PriorityQueue/
Lab12-Deque/
Lab13-Hash/
```

(There is no Lab 11 — the course skips it.)

---

## One-time setup

Do this once, at the start of the semester.

### If you are the teammate (not the repo owner)

1. Go to the upstream repo on GitHub and click **Fork**.
2. Clone **your fork**:
   ```bash
   git clone https://github.com/YOUR-USERNAME/CSE-232-Labs.git
   cd CSE-232-Labs
   ```
3. Point at the upstream so you can pull in the other person's merged work:
   ```bash
   git remote add upstream https://github.com/Willpatbarr/CSE-232-Labs.git
   ```
4. Check it worked — you should see `origin` (your fork) and `upstream`:
   ```bash
   git remote -v
   ```

### If you are the repo owner

GitHub won't let you fork your own repo, so just clone it directly. Every
other step below is identical for you — you still work on a branch, you
still open a PR, you never push straight to `main`.

```bash
git clone https://github.com/Willpatbarr/CSE-232-Labs.git
cd CSE-232-Labs
git remote add upstream https://github.com/Willpatbarr/CSE-232-Labs.git
```

> If you'd rather both of you have real forks, move the repo into a GitHub
> organization and fork from there. Not required.

---

## Working a lab

**One branch per lab.** Not one per edit. The branch is named after the
lab folder, lowercased:

| Lab folder | Branch name |
|---|---|
| `Lab02-Vector` | `lab02-vector` |
| `Lab03-Stack` | `lab03-stack` |
| `Lab07-BST` | `lab07-bst` |

### 1. The branch gets made on upstream, once

Whoever starts the lab first creates it on the upstream repo. Easiest way
is on GitHub: **branches → New branch**, name it `lab02-vector`, source
`main`.

From the command line (owner only):

```bash
git checkout main
git pull upstream main
git checkout -b lab02-vector
git push upstream lab02-vector
```

### 2. Both of you check that branch out

```bash
git fetch upstream
git checkout -b lab02-vector upstream/lab02-vector
```

### 3. Work, commit, push to your own fork

```bash
git add .
git commit -m "vector: implement push_back and resize"
git push origin lab02-vector
```

### 4. Open a PR into the lab branch

On GitHub: **New pull request**.

- base repository: the **upstream** repo, base branch: `lab02-vector`
- head repository: **your fork**, compare branch: `lab02-vector`

The other person reviews it and merges. Now both of your work lives on
`lab02-vector`.

### 5. Pull the other person's merged work before you keep going

```bash
git fetch upstream
git merge upstream/lab02-vector
```

Do this **before** you start a new chunk of work, not after. It is the
single best way to avoid painful conflicts.

### 6. When the lab is done and passing, merge the lab into main

One final PR on the upstream repo:

- base: `main`
- compare: `lab02-vector`

Merge it. Then delete the lab branch. Start the next lab from step 1.

---

## Rules

1. **Never push to `main`.** Everything reaches `main` through a PR.
2. **One branch per lab**, not per change. Small commits on that branch are
   fine and encouraged.
3. **Pull before you start working**, every session. Step 5 above.
4. **Review each other's PRs.** Even a "looks good" comment — the point of
   this repo is partly to get comfortable with the PR loop.
5. **Commit messages say what changed**, not "update". `vector: fix
   iterator end()` beats `changes`.

---

## Windows + Mac in the same repo

She's on Visual Studio (Windows), he's on Xcode (Mac). Two things are set
up so that isn't painful:

**`.gitattributes`** normalizes line endings. Without it, Windows CRLF vs
Mac LF makes every file look 100% rewritten to the other person.

**`.gitignore`** drops everything an IDE writes *about you* rather than
about the project — window layouts, breakpoints, `DerivedData/`, `.vs/`,
`x64/`, `Debug/`, `*.xcuserstate`. None of that belongs in git and all of
it used to cause noise.

### The one thing you have to remember

Visual Studio and Xcode each keep **their own list of which files are in
the project**:

- Visual Studio's list lives in `LabVector.vcxproj`
- Xcode's list lives in `LabVector.xcodeproj/project.pbxproj`

Adding a file in one IDE only updates that IDE's list. The file will show
up in git for the other person but **won't appear in their project**, and
their build will fail with a missing symbol.

**So: if you add a brand-new `.h` or `.cpp` file, say so in the PR
description.** The other person then adds it to their own IDE once —
in Xcode, drag it into the project navigator; in Visual Studio, right-click
the project → Add → Existing Item. One click, done.

This barely comes up. These labs hand you every file you need; you're
filling in function bodies, not adding files. It's just the one thing to
watch for.

### If `project.pbxproj` ever conflicts

Don't try to hand-merge it. Take one side whole and re-add anything
missing through the IDE:

```bash
git checkout --theirs LabVector.xcodeproj/project.pbxproj
git add LabVector.xcodeproj/project.pbxproj
```

---

## Quick reference

```bash
# start of every work session
git fetch upstream
git merge upstream/lab02-vector

# save work
git add .
git commit -m "vector: implement erase"
git push origin lab02-vector

# then open a PR on GitHub into upstream/lab02-vector
```
