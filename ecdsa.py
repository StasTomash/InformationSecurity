import random
import hashlib

# https://datatracker.ietf.org/doc/html/rfc5639#section-3.7 (brainpoolP512r1 curve)

DEFAULT_CURVE_BASE = int("AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA703308717D4D9B009BC66842AECDA12AE6A380E62881FF2F2D82C68528AA6056583A48F3", 16)
DEFAULT_CURVE_PARAM_A = int("7830A3318B603B89E2327145AC234CC594CBDD8D3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CA", 16)
DEFAULT_CURVE_PARAM_B = int("3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CADC083E67984050B75EBAE5DD2809BD638016F723", 16)
DEFAULT_X = int("81AEE4BDD82ED9645A21322E9C4C6A9385ED9F70B5D916C1B43B62EEF4D0098EFF3B1F78E2D0D48D50D1687B93B97D5F7C6D5047406A5E688B352209BCB9F822", 16)
DEFAULT_Y = int("7DDE385D566332ECC0EABFA9CF7822FDF209F70024A57B1AA000C55B881F8111B2DCDE494A5F485E5BCA4BD88A2763AED1CA2B2FA8F0540678CD1E0F3AD80892", 16)
ORDER = int("AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA70330870553E5C414CA92619418661197FAC10471DB1D381085DDADDB58796829CA90069", 16)


class EllipticCurve:
    def __init__(self):
        self.a = DEFAULT_CURVE_PARAM_A
        self.b = DEFAULT_CURVE_PARAM_B
        self.p = DEFAULT_CURVE_BASE

    def gen_key_pair(self):
        seed = random.randrange(self.p)
        base_point = Point(self)
        q = base_point * seed
        return q, seed


def gcd_extended(a, b):
    if a == 0:
        return b, 0, 1
    d, x1, y1 = gcd_extended((b % a + a) % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return d, x, y


def inverse_in_circle(arg, modulo):
    g, x, y = gcd_extended(arg, modulo)
    if g != 1:
        raise ValueError
    return (x % modulo + modulo) % modulo


class Point:
    def __init__(self, curve: EllipticCurve, x=DEFAULT_X, y=DEFAULT_Y):
        self.x = x
        self.y = y
        self.parent = curve

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.parent == other.parent

    def __add__(self, other):
        ans = Point(self.parent, 0, 0)
        if self.x == 0 and self.y == 0:
            return other
        if other.x == 0 and other.y == 0:
            return other
        if self.x == other.x:
            slope = (3 * self.x * self.x + self.parent.a) * inverse_in_circle(2 * self.y, self.parent.p)
            slope = (slope % self.parent.p + self.parent.p) % self.parent.p
        else:
            slope = (other.y - self.y) * inverse_in_circle(((other.x - self.x) % self.parent.p + self.parent.p) % self.parent.p, self.parent.p)
            slope = (slope % self.parent.p + self.parent.p) % self.parent.p

        ans.x = ((slope ** 2 - self.x - other.x) % self.parent.p + self.parent.p) % self.parent.p
        ans.y = (((self.x - ans.x) * slope - self.y) % self.parent.p + self.parent.p) % self.parent.p
        return ans

    def __str__(self):
        return f"[{self.parent.a, self.parent.b, self.parent.p}], {self.x}, {self.y}"

    def __mul__(self, other):
        if other == 1:
            return self
        if other == 0:
            return Point(self.parent, 0, 0)
        ans = Point(self.parent, 0, 0)
        cp = Point(self.parent, self.x, self.y)
        exp = other
        while exp != 0:
            if exp % 2 == 1:
                ans = ans + cp
            cp = cp + cp
            exp //= 2
        return ans


def get_hash(msg):
    h = hashlib.sha256()
    h.update(msg.encode('utf-8'))
    return int(h.hexdigest(), 16)


def sign(curve: EllipticCurve, msg, private_key):
    h = get_hash(msg) % ORDER
    while True:
        k = random.randrange(ORDER)
        p = Point(curve) * k
        r = (p.x % ORDER + ORDER) % ORDER
        if r == 0:
            continue
        s = ((inverse_in_circle(k, ORDER) * (h + r * private_key)) % ORDER + ORDER) % ORDER
        if s == 0:
            continue
        return r, s


def verify_signature(curve, signature, msg, public_key):
    r = signature[0]
    s = signature[1]
    h = get_hash(msg) % ORDER
    inv = inverse_in_circle(s, ORDER)
    u1 = ((h * inv) % ORDER + ORDER) % ORDER
    u2 = ((r * inv) % ORDER + ORDER) % ORDER
    p = Point(curve) * u1 + public_key * u2
    return r % ORDER == p.x % ORDER


if __name__ == '__main__':
    curve = EllipticCurve()
    public_key, private_key = curve.gen_key_pair()
    other_public_key, other_private_key = curve.gen_key_pair()
    msg = "Test message"
    signature = sign(curve, msg, private_key)
    print("Correct Private Key, Correct Public Key: \tVerification", verify_signature(curve, signature, msg, public_key))
    signature = sign(curve, msg, other_private_key)
    print("Wrong Private Key, Correct Public Key: \t\tVerification", verify_signature(curve, signature, msg, public_key))
    signature = sign(curve, msg, private_key)
    print("Correct Private Key, Wrong Public Key: \t\tVerification", verify_signature(curve, signature, msg, other_public_key))
